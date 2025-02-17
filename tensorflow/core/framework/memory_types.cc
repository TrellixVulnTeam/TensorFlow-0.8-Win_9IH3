/* Copyright 2015 Google Inc. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "tensorflow/core/framework/memory_types.h"

#include "tensorflow/core/framework/kernel_def.pb.h"
#include "tensorflow/core/framework/node_def_util.h"
#include "tensorflow/core/framework/op_kernel.h"
#include "tensorflow/core/framework/types.h"
#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/platform/types.h"

#ifdef _MSC_VER
#undef max
#endif

namespace tensorflow {

namespace {
// Returns the largest endpoint of anything in the name_map.
int GetTotal(const NameRangeMap& name_map) {
  int total = 0;
  for (const auto& item : name_map) {
    total = std::max(total, item.second.second);
  }
  return total;
}

// Fills memory_types for either input or output, setting everything
// to DEVICE_MEMORY except those args in host_memory_args.  Removes
// elements of host_memory_args that were used.
void MemoryTypesHelper(const NameRangeMap& name_map,
                       std::vector<string>* host_memory_args,
                       MemoryTypeVector* memory_types) {
  // Now that we know the size, fill with the default 'DEVICE_MEMORY'.
  memory_types->clear();
  memory_types->resize(GetTotal(name_map), DEVICE_MEMORY);

  // Update args that have been marked as in "HOST_MEMORY".
  size_t keep = 0;
  for (size_t i = 0; i < host_memory_args->size(); ++i) {
    auto iter = name_map.find((*host_memory_args)[i]);
    if (iter != name_map.end()) {
      for (int j = iter->second.first; j < iter->second.second; ++j) {
        (*memory_types)[j] = HOST_MEMORY;
      }
    } else {
      // (*host_memory_args)[i] not found, save it for the next pass.
      if (i > keep) (*host_memory_args)[keep] = (*host_memory_args)[i];
      ++keep;
    }
  }
  host_memory_args->resize(keep);
}

MemoryType MTypeFromDType(const DataType dtype) {
  return (dtype == DT_INT32) ? HOST_MEMORY : DEVICE_MEMORY;
}

// Returns true if an arg of op_def's input/output is a type list.
bool HasTypeList(const OpDef& op_def) {
  for (const auto& a : op_def.input_arg()) {
    if (!a.type_list_attr().empty()) return true;
  }
  for (const auto& a : op_def.output_arg()) {
    if (!a.type_list_attr().empty()) return true;
  }
  return false;
}

}  // namespace

Status MemoryTypesForNode(const OpRegistryInterface* op_registry,
                          DeviceType device_type, const NodeDef& ndef,
                          MemoryTypeVector* inp_mtypes,
                          MemoryTypeVector* out_mtypes) {
  // Look up the Op registered for this op name.
  Status status;
  const OpDef* op_def = op_registry->LookUp(ndef.op(), &status);
  if (op_def == nullptr) return status;

  // Look up the Kernel registered for this node def.
  const KernelDef* kdef = nullptr;
  status =
      FindKernelDef(device_type, ndef, &kdef, nullptr /* kernel_class_name */);

  if (!status.ok() || HasTypeList(*op_def)) {
    // When there is no kernel def for this op or the op's arg is a
    // type list, we can only best-effort derive the memory type from
    // the data type.  For now, we assume int32 is always on host
    // memory and other types are always on device memory. We should
    // do type inference over function body to derive the correct
    // input/output memory types.
    DataTypeVector inp_dtypes;
    DataTypeVector out_dtypes;
    TF_RETURN_IF_ERROR(
        InOutTypesForNode(ndef, *op_def, &inp_dtypes, &out_dtypes));
    inp_mtypes->clear();
    for (const auto& t : inp_dtypes) inp_mtypes->push_back(MTypeFromDType(t));
    out_mtypes->clear();
    for (const auto& t : out_dtypes) out_mtypes->push_back(MTypeFromDType(t));
    return Status::OK();
  }

  // Gets the input/output names and their corresponding endpoint ranges.
  NameRangeMap inp_names;
  NameRangeMap out_names;
  TF_RETURN_IF_ERROR(NameRangesForNode(ndef, *op_def, &inp_names, &out_names));

  // Fills in host memory types based on the kernel def.
  const auto& from_proto = kdef->host_memory_arg();
  std::vector<string> host_memory_args(from_proto.begin(), from_proto.end());
  MemoryTypesHelper(inp_names, &host_memory_args, inp_mtypes);
  MemoryTypesHelper(out_names, &host_memory_args, out_mtypes);
  if (!host_memory_args.empty()) {
    return errors::InvalidArgument(
        "HostMemory args '", str_util::Join(host_memory_args, "', '"),
        "' not found in OpDef: ", SummarizeOpDef(*op_def));
  }
  return Status::OK();
}

}  // namespace tensorflow
