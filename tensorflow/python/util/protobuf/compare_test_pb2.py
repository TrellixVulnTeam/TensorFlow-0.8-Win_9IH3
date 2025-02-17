# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: tensorflow/python/util/protobuf/compare_test.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf.internal import enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='tensorflow/python/util/protobuf/compare_test.proto',
  package='compare_test',
  syntax='proto2',
  serialized_pb=_b('\n2tensorflow/python/util/protobuf/compare_test.proto\x12\x0c\x63ompare_test\"\x18\n\x05Small\x12\x0f\n\x07strings\x18\x01 \x03(\t\"\xd3\x01\n\x06Medium\x12\x0e\n\x06int32s\x18\x01 \x03(\x05\x12#\n\x06smalls\x18\x02 \x03(\x0b\x32\x13.compare_test.Small\x12+\n\x06groupa\x18\x03 \x03(\n2\x1b.compare_test.Medium.GroupA\x12\x0e\n\x06\x66loats\x18\x06 \x03(\x02\x1aW\n\x06GroupA\x12\x32\n\x06groupb\x18\x04 \x03(\n2\".compare_test.Medium.GroupA.GroupB\x1a\x19\n\x06GroupB\x12\x0f\n\x07strings\x18\x05 \x02(\t\"\xfe\x01\n\x05Large\x12\x0f\n\x07string_\x18\x01 \x01(\t\x12\x0e\n\x06int64_\x18\x02 \x01(\x03\x12\x0e\n\x06\x66loat_\x18\x03 \x01(\x02\x12\r\n\x05\x62ool_\x18\x04 \x01(\x08\x12!\n\x05\x65num_\x18\x05 \x01(\x0e\x32\x12.compare_test.Enum\x12\x0e\n\x06int64s\x18\x06 \x03(\x03\x12$\n\x06medium\x18\x07 \x01(\x0b\x32\x14.compare_test.Medium\x12\"\n\x05small\x18\x08 \x01(\x0b\x32\x13.compare_test.Small\x12\x0f\n\x07\x64ouble_\x18\t \x01(\x01\x12\'\n\x08with_map\x18\n \x01(\x0b\x32\x15.compare_test.WithMap\"-\n\x07Labeled\x12\x10\n\x08required\x18\x01 \x02(\x05\x12\x10\n\x08optional\x18\x02 \x01(\x05\"\x85\x02\n\x07WithMap\x12>\n\rvalue_message\x18\x01 \x03(\x0b\x32\'.compare_test.WithMap.ValueMessageEntry\x12<\n\x0cvalue_string\x18\x02 \x03(\x0b\x32&.compare_test.WithMap.ValueStringEntry\x1aH\n\x11ValueMessageEntry\x12\x0b\n\x03key\x18\x01 \x01(\x05\x12\"\n\x05value\x18\x02 \x01(\x0b\x32\x13.compare_test.Small:\x02\x38\x01\x1a\x32\n\x10ValueStringEntry\x12\x0b\n\x03key\x18\x01 \x01(\t\x12\r\n\x05value\x18\x02 \x01(\t:\x02\x38\x01*\x1b\n\x04\x45num\x12\x05\n\x01\x41\x10\x00\x12\x05\n\x01\x42\x10\x01\x12\x05\n\x01\x43\x10\x02')
)
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

_ENUM = _descriptor.EnumDescriptor(
  name='Enum',
  full_name='compare_test.Enum',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='A', index=0, number=0,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='B', index=1, number=1,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='C', index=2, number=2,
      options=None,
      type=None),
  ],
  containing_type=None,
  options=None,
  serialized_start=876,
  serialized_end=903,
)
_sym_db.RegisterEnumDescriptor(_ENUM)

Enum = enum_type_wrapper.EnumTypeWrapper(_ENUM)
A = 0
B = 1
C = 2



_SMALL = _descriptor.Descriptor(
  name='Small',
  full_name='compare_test.Small',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='strings', full_name='compare_test.Small.strings', index=0,
      number=1, type=9, cpp_type=9, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=68,
  serialized_end=92,
)


_MEDIUM_GROUPA_GROUPB = _descriptor.Descriptor(
  name='GroupB',
  full_name='compare_test.Medium.GroupA.GroupB',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='strings', full_name='compare_test.Medium.GroupA.GroupB.strings', index=0,
      number=5, type=9, cpp_type=9, label=2,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=281,
  serialized_end=306,
)

_MEDIUM_GROUPA = _descriptor.Descriptor(
  name='GroupA',
  full_name='compare_test.Medium.GroupA',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='groupb', full_name='compare_test.Medium.GroupA.groupb', index=0,
      number=4, type=10, cpp_type=10, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[_MEDIUM_GROUPA_GROUPB, ],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=219,
  serialized_end=306,
)

_MEDIUM = _descriptor.Descriptor(
  name='Medium',
  full_name='compare_test.Medium',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='int32s', full_name='compare_test.Medium.int32s', index=0,
      number=1, type=5, cpp_type=1, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='smalls', full_name='compare_test.Medium.smalls', index=1,
      number=2, type=11, cpp_type=10, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='groupa', full_name='compare_test.Medium.groupa', index=2,
      number=3, type=10, cpp_type=10, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='floats', full_name='compare_test.Medium.floats', index=3,
      number=6, type=2, cpp_type=6, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[_MEDIUM_GROUPA, ],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=95,
  serialized_end=306,
)


_LARGE = _descriptor.Descriptor(
  name='Large',
  full_name='compare_test.Large',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='string_', full_name='compare_test.Large.string_', index=0,
      number=1, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='int64_', full_name='compare_test.Large.int64_', index=1,
      number=2, type=3, cpp_type=2, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='float_', full_name='compare_test.Large.float_', index=2,
      number=3, type=2, cpp_type=6, label=1,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='bool_', full_name='compare_test.Large.bool_', index=3,
      number=4, type=8, cpp_type=7, label=1,
      has_default_value=False, default_value=False,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='enum_', full_name='compare_test.Large.enum_', index=4,
      number=5, type=14, cpp_type=8, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='int64s', full_name='compare_test.Large.int64s', index=5,
      number=6, type=3, cpp_type=2, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='medium', full_name='compare_test.Large.medium', index=6,
      number=7, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='small', full_name='compare_test.Large.small', index=7,
      number=8, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='double_', full_name='compare_test.Large.double_', index=8,
      number=9, type=1, cpp_type=5, label=1,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='with_map', full_name='compare_test.Large.with_map', index=9,
      number=10, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=309,
  serialized_end=563,
)


_LABELED = _descriptor.Descriptor(
  name='Labeled',
  full_name='compare_test.Labeled',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='required', full_name='compare_test.Labeled.required', index=0,
      number=1, type=5, cpp_type=1, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='optional', full_name='compare_test.Labeled.optional', index=1,
      number=2, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=565,
  serialized_end=610,
)


_WITHMAP_VALUEMESSAGEENTRY = _descriptor.Descriptor(
  name='ValueMessageEntry',
  full_name='compare_test.WithMap.ValueMessageEntry',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='key', full_name='compare_test.WithMap.ValueMessageEntry.key', index=0,
      number=1, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='value', full_name='compare_test.WithMap.ValueMessageEntry.value', index=1,
      number=2, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=_descriptor._ParseOptions(descriptor_pb2.MessageOptions(), _b('8\001')),
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=750,
  serialized_end=822,
)

_WITHMAP_VALUESTRINGENTRY = _descriptor.Descriptor(
  name='ValueStringEntry',
  full_name='compare_test.WithMap.ValueStringEntry',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='key', full_name='compare_test.WithMap.ValueStringEntry.key', index=0,
      number=1, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='value', full_name='compare_test.WithMap.ValueStringEntry.value', index=1,
      number=2, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=_descriptor._ParseOptions(descriptor_pb2.MessageOptions(), _b('8\001')),
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=824,
  serialized_end=874,
)

_WITHMAP = _descriptor.Descriptor(
  name='WithMap',
  full_name='compare_test.WithMap',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='value_message', full_name='compare_test.WithMap.value_message', index=0,
      number=1, type=11, cpp_type=10, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='value_string', full_name='compare_test.WithMap.value_string', index=1,
      number=2, type=11, cpp_type=10, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[_WITHMAP_VALUEMESSAGEENTRY, _WITHMAP_VALUESTRINGENTRY, ],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=613,
  serialized_end=874,
)

_MEDIUM_GROUPA_GROUPB.containing_type = _MEDIUM_GROUPA
_MEDIUM_GROUPA.fields_by_name['groupb'].message_type = _MEDIUM_GROUPA_GROUPB
_MEDIUM_GROUPA.containing_type = _MEDIUM
_MEDIUM.fields_by_name['smalls'].message_type = _SMALL
_MEDIUM.fields_by_name['groupa'].message_type = _MEDIUM_GROUPA
_LARGE.fields_by_name['enum_'].enum_type = _ENUM
_LARGE.fields_by_name['medium'].message_type = _MEDIUM
_LARGE.fields_by_name['small'].message_type = _SMALL
_LARGE.fields_by_name['with_map'].message_type = _WITHMAP
_WITHMAP_VALUEMESSAGEENTRY.fields_by_name['value'].message_type = _SMALL
_WITHMAP_VALUEMESSAGEENTRY.containing_type = _WITHMAP
_WITHMAP_VALUESTRINGENTRY.containing_type = _WITHMAP
_WITHMAP.fields_by_name['value_message'].message_type = _WITHMAP_VALUEMESSAGEENTRY
_WITHMAP.fields_by_name['value_string'].message_type = _WITHMAP_VALUESTRINGENTRY
DESCRIPTOR.message_types_by_name['Small'] = _SMALL
DESCRIPTOR.message_types_by_name['Medium'] = _MEDIUM
DESCRIPTOR.message_types_by_name['Large'] = _LARGE
DESCRIPTOR.message_types_by_name['Labeled'] = _LABELED
DESCRIPTOR.message_types_by_name['WithMap'] = _WITHMAP
DESCRIPTOR.enum_types_by_name['Enum'] = _ENUM

Small = _reflection.GeneratedProtocolMessageType('Small', (_message.Message,), dict(
  DESCRIPTOR = _SMALL,
  __module__ = 'tensorflow.python.util.protobuf.compare_test_pb2'
  # @@protoc_insertion_point(class_scope:compare_test.Small)
  ))
_sym_db.RegisterMessage(Small)

Medium = _reflection.GeneratedProtocolMessageType('Medium', (_message.Message,), dict(

  GroupA = _reflection.GeneratedProtocolMessageType('GroupA', (_message.Message,), dict(

    GroupB = _reflection.GeneratedProtocolMessageType('GroupB', (_message.Message,), dict(
      DESCRIPTOR = _MEDIUM_GROUPA_GROUPB,
      __module__ = 'tensorflow.python.util.protobuf.compare_test_pb2'
      # @@protoc_insertion_point(class_scope:compare_test.Medium.GroupA.GroupB)
      ))
    ,
    DESCRIPTOR = _MEDIUM_GROUPA,
    __module__ = 'tensorflow.python.util.protobuf.compare_test_pb2'
    # @@protoc_insertion_point(class_scope:compare_test.Medium.GroupA)
    ))
  ,
  DESCRIPTOR = _MEDIUM,
  __module__ = 'tensorflow.python.util.protobuf.compare_test_pb2'
  # @@protoc_insertion_point(class_scope:compare_test.Medium)
  ))
_sym_db.RegisterMessage(Medium)
_sym_db.RegisterMessage(Medium.GroupA)
_sym_db.RegisterMessage(Medium.GroupA.GroupB)

Large = _reflection.GeneratedProtocolMessageType('Large', (_message.Message,), dict(
  DESCRIPTOR = _LARGE,
  __module__ = 'tensorflow.python.util.protobuf.compare_test_pb2'
  # @@protoc_insertion_point(class_scope:compare_test.Large)
  ))
_sym_db.RegisterMessage(Large)

Labeled = _reflection.GeneratedProtocolMessageType('Labeled', (_message.Message,), dict(
  DESCRIPTOR = _LABELED,
  __module__ = 'tensorflow.python.util.protobuf.compare_test_pb2'
  # @@protoc_insertion_point(class_scope:compare_test.Labeled)
  ))
_sym_db.RegisterMessage(Labeled)

WithMap = _reflection.GeneratedProtocolMessageType('WithMap', (_message.Message,), dict(

  ValueMessageEntry = _reflection.GeneratedProtocolMessageType('ValueMessageEntry', (_message.Message,), dict(
    DESCRIPTOR = _WITHMAP_VALUEMESSAGEENTRY,
    __module__ = 'tensorflow.python.util.protobuf.compare_test_pb2'
    # @@protoc_insertion_point(class_scope:compare_test.WithMap.ValueMessageEntry)
    ))
  ,

  ValueStringEntry = _reflection.GeneratedProtocolMessageType('ValueStringEntry', (_message.Message,), dict(
    DESCRIPTOR = _WITHMAP_VALUESTRINGENTRY,
    __module__ = 'tensorflow.python.util.protobuf.compare_test_pb2'
    # @@protoc_insertion_point(class_scope:compare_test.WithMap.ValueStringEntry)
    ))
  ,
  DESCRIPTOR = _WITHMAP,
  __module__ = 'tensorflow.python.util.protobuf.compare_test_pb2'
  # @@protoc_insertion_point(class_scope:compare_test.WithMap)
  ))
_sym_db.RegisterMessage(WithMap)
_sym_db.RegisterMessage(WithMap.ValueMessageEntry)
_sym_db.RegisterMessage(WithMap.ValueStringEntry)


_WITHMAP_VALUEMESSAGEENTRY.has_options = True
_WITHMAP_VALUEMESSAGEENTRY._options = _descriptor._ParseOptions(descriptor_pb2.MessageOptions(), _b('8\001'))
_WITHMAP_VALUESTRINGENTRY.has_options = True
_WITHMAP_VALUESTRINGENTRY._options = _descriptor._ParseOptions(descriptor_pb2.MessageOptions(), _b('8\001'))
# @@protoc_insertion_point(module_scope)
