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

#ifdef _WIN32
#include <Shlwapi.h>
#endif

#include "tensorflow/core/lib/io/path.h"
#include "tensorflow/core/lib/strings/strcat.h"

namespace tensorflow {
namespace io {

#ifdef _WIN32
inline bool StartWithSeperator(const StringPiece& path) {
	return !path.empty() && (path[0] == '/' || path[0] == '\\');
}
#endif

string JoinPath(StringPiece part1, StringPiece part2) {
  string result;

  StringPiece paths[2] = {part1, part2};
  for (StringPiece path : paths) {
    if (path.empty()) continue;

    if (result.empty()) {
      result = path.ToString();
      continue;
    }

#ifndef _WIN32
    if (result[result.size() - 1] == '/') {
      if (IsAbsolutePath(path)) {
        strings::StrAppend(&result, path.substr(1));
      } else {
        strings::StrAppend(&result, path);
      }
    } else {
      if (IsAbsolutePath(path)) {
        strings::StrAppend(&result, path);
      } else {
        strings::StrAppend(&result, "/", path);
      }
    }
#else
    if (result[result.size() - 1] == '/' || result[result.size() - 1] == '\\') {
      if (StartWithSeperator(path)) {
        strings::StrAppend(&result, path.substr(1));
      }
      else {
        strings::StrAppend(&result, path);
      }
    } else {
      if (StartWithSeperator(path)) {
        strings::StrAppend(&result, path);
      }
      else {
        strings::StrAppend(&result, "/", path);
      }
    }
#endif
  }

  return result;
}

namespace internal {

// Return the parts of the path, split on the final "/".  If there is no
// "/" in the path, the first part of the output is empty and the second
// is the input. If the only "/" in the path is the first character, it is
// the first part of the output.
std::pair<StringPiece, StringPiece> SplitPath(StringPiece path) {
#ifndef _WIN32
  auto pos = path.rfind('/');
#else
  auto pos1 = path.rfind('/');
  auto pos2 = path.rfind('\\');

  decltype(pos2) pos = 0;
  if (pos1 == StringPiece::npos || pos2 == StringPiece::npos) {
    pos = (std::min)(pos1, pos2);
  } else {
    pos = (std::max)(pos1, pos2);
  }
#endif

  // Handle the case with no '/' in 'path'.
  if (pos == StringPiece::npos)
    return std::make_pair(StringPiece(path.data(), 0), path);

  // Handle the case with a single leading '/' in 'path'.
  if (pos == 0)
    return std::make_pair(StringPiece(path.data(), 1),
                          StringPiece(path.data() + 1, path.size() - 1));

  return std::make_pair(
      StringPiece(path.data(), pos),
      StringPiece(path.data() + pos + 1, path.size() - (pos + 1)));
}

// Return the parts of the basename of path, split on the final ".".
// If there is no "." in the basename or "." is the final character in the
// basename, the second value will be empty.
std::pair<StringPiece, StringPiece> SplitBasename(StringPiece path) {
  path = Basename(path);

  auto pos = path.rfind('.');
  if (pos == StringPiece::npos)
    return std::make_pair(path, StringPiece(path.data() + path.size(), 0));
  return std::make_pair(
      StringPiece(path.data(), pos),
      StringPiece(path.data() + pos + 1, path.size() - (pos + 1)));
}
}  // namespace internal

bool IsAbsolutePath(StringPiece path) {
#ifndef _WIN32
  return !path.empty() && path[0] == '/';
#else
  return !PathIsRelative(path.data());
#endif
}

StringPiece Dirname(StringPiece path) {
  return internal::SplitPath(path).first;
}

StringPiece Basename(StringPiece path) {
  return internal::SplitPath(path).second;
}

StringPiece Extension(StringPiece path) {
  return internal::SplitBasename(path).second;
}

}  // namespace io
}  // namespace tensorflow
