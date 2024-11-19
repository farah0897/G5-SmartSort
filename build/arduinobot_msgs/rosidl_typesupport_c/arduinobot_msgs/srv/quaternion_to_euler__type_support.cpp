// generated from rosidl_typesupport_c/resource/idl__type_support.cpp.em
// with input from arduinobot_msgs:srv/QuaternionToEuler.idl
// generated code does not contain a copyright notice

#include "cstddef"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "arduinobot_msgs/srv/detail/quaternion_to_euler__struct.h"
#include "arduinobot_msgs/srv/detail/quaternion_to_euler__type_support.h"
#include "arduinobot_msgs/srv/detail/quaternion_to_euler__functions.h"
#include "rosidl_typesupport_c/identifier.h"
#include "rosidl_typesupport_c/message_type_support_dispatch.h"
#include "rosidl_typesupport_c/type_support_map.h"
#include "rosidl_typesupport_c/visibility_control.h"
#include "rosidl_typesupport_interface/macros.h"

namespace arduinobot_msgs
{

namespace srv
{

namespace rosidl_typesupport_c
{

typedef struct _QuaternionToEuler_Request_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _QuaternionToEuler_Request_type_support_ids_t;

static const _QuaternionToEuler_Request_type_support_ids_t _QuaternionToEuler_Request_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_c",  // ::rosidl_typesupport_fastrtps_c::typesupport_identifier,
    "rosidl_typesupport_introspection_c",  // ::rosidl_typesupport_introspection_c::typesupport_identifier,
  }
};

typedef struct _QuaternionToEuler_Request_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _QuaternionToEuler_Request_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _QuaternionToEuler_Request_type_support_symbol_names_t _QuaternionToEuler_Request_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, arduinobot_msgs, srv, QuaternionToEuler_Request)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, arduinobot_msgs, srv, QuaternionToEuler_Request)),
  }
};

typedef struct _QuaternionToEuler_Request_type_support_data_t
{
  void * data[2];
} _QuaternionToEuler_Request_type_support_data_t;

static _QuaternionToEuler_Request_type_support_data_t _QuaternionToEuler_Request_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _QuaternionToEuler_Request_message_typesupport_map = {
  2,
  "arduinobot_msgs",
  &_QuaternionToEuler_Request_message_typesupport_ids.typesupport_identifier[0],
  &_QuaternionToEuler_Request_message_typesupport_symbol_names.symbol_name[0],
  &_QuaternionToEuler_Request_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t QuaternionToEuler_Request_message_type_support_handle = {
  rosidl_typesupport_c__typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_QuaternionToEuler_Request_message_typesupport_map),
  rosidl_typesupport_c__get_message_typesupport_handle_function,
  &arduinobot_msgs__srv__QuaternionToEuler_Request__get_type_hash,
  &arduinobot_msgs__srv__QuaternionToEuler_Request__get_type_description,
  &arduinobot_msgs__srv__QuaternionToEuler_Request__get_type_description_sources,
};

}  // namespace rosidl_typesupport_c

}  // namespace srv

}  // namespace arduinobot_msgs

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_c, arduinobot_msgs, srv, QuaternionToEuler_Request)() {
  return &::arduinobot_msgs::srv::rosidl_typesupport_c::QuaternionToEuler_Request_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "cstddef"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "arduinobot_msgs/srv/detail/quaternion_to_euler__struct.h"
// already included above
// #include "arduinobot_msgs/srv/detail/quaternion_to_euler__type_support.h"
// already included above
// #include "arduinobot_msgs/srv/detail/quaternion_to_euler__functions.h"
// already included above
// #include "rosidl_typesupport_c/identifier.h"
// already included above
// #include "rosidl_typesupport_c/message_type_support_dispatch.h"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_c/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace arduinobot_msgs
{

namespace srv
{

namespace rosidl_typesupport_c
{

typedef struct _QuaternionToEuler_Response_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _QuaternionToEuler_Response_type_support_ids_t;

static const _QuaternionToEuler_Response_type_support_ids_t _QuaternionToEuler_Response_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_c",  // ::rosidl_typesupport_fastrtps_c::typesupport_identifier,
    "rosidl_typesupport_introspection_c",  // ::rosidl_typesupport_introspection_c::typesupport_identifier,
  }
};

typedef struct _QuaternionToEuler_Response_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _QuaternionToEuler_Response_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _QuaternionToEuler_Response_type_support_symbol_names_t _QuaternionToEuler_Response_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, arduinobot_msgs, srv, QuaternionToEuler_Response)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, arduinobot_msgs, srv, QuaternionToEuler_Response)),
  }
};

typedef struct _QuaternionToEuler_Response_type_support_data_t
{
  void * data[2];
} _QuaternionToEuler_Response_type_support_data_t;

static _QuaternionToEuler_Response_type_support_data_t _QuaternionToEuler_Response_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _QuaternionToEuler_Response_message_typesupport_map = {
  2,
  "arduinobot_msgs",
  &_QuaternionToEuler_Response_message_typesupport_ids.typesupport_identifier[0],
  &_QuaternionToEuler_Response_message_typesupport_symbol_names.symbol_name[0],
  &_QuaternionToEuler_Response_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t QuaternionToEuler_Response_message_type_support_handle = {
  rosidl_typesupport_c__typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_QuaternionToEuler_Response_message_typesupport_map),
  rosidl_typesupport_c__get_message_typesupport_handle_function,
  &arduinobot_msgs__srv__QuaternionToEuler_Response__get_type_hash,
  &arduinobot_msgs__srv__QuaternionToEuler_Response__get_type_description,
  &arduinobot_msgs__srv__QuaternionToEuler_Response__get_type_description_sources,
};

}  // namespace rosidl_typesupport_c

}  // namespace srv

}  // namespace arduinobot_msgs

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_c, arduinobot_msgs, srv, QuaternionToEuler_Response)() {
  return &::arduinobot_msgs::srv::rosidl_typesupport_c::QuaternionToEuler_Response_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "cstddef"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "arduinobot_msgs/srv/detail/quaternion_to_euler__struct.h"
// already included above
// #include "arduinobot_msgs/srv/detail/quaternion_to_euler__type_support.h"
// already included above
// #include "arduinobot_msgs/srv/detail/quaternion_to_euler__functions.h"
// already included above
// #include "rosidl_typesupport_c/identifier.h"
// already included above
// #include "rosidl_typesupport_c/message_type_support_dispatch.h"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_c/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace arduinobot_msgs
{

namespace srv
{

namespace rosidl_typesupport_c
{

typedef struct _QuaternionToEuler_Event_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _QuaternionToEuler_Event_type_support_ids_t;

static const _QuaternionToEuler_Event_type_support_ids_t _QuaternionToEuler_Event_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_c",  // ::rosidl_typesupport_fastrtps_c::typesupport_identifier,
    "rosidl_typesupport_introspection_c",  // ::rosidl_typesupport_introspection_c::typesupport_identifier,
  }
};

typedef struct _QuaternionToEuler_Event_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _QuaternionToEuler_Event_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _QuaternionToEuler_Event_type_support_symbol_names_t _QuaternionToEuler_Event_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, arduinobot_msgs, srv, QuaternionToEuler_Event)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, arduinobot_msgs, srv, QuaternionToEuler_Event)),
  }
};

typedef struct _QuaternionToEuler_Event_type_support_data_t
{
  void * data[2];
} _QuaternionToEuler_Event_type_support_data_t;

static _QuaternionToEuler_Event_type_support_data_t _QuaternionToEuler_Event_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _QuaternionToEuler_Event_message_typesupport_map = {
  2,
  "arduinobot_msgs",
  &_QuaternionToEuler_Event_message_typesupport_ids.typesupport_identifier[0],
  &_QuaternionToEuler_Event_message_typesupport_symbol_names.symbol_name[0],
  &_QuaternionToEuler_Event_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t QuaternionToEuler_Event_message_type_support_handle = {
  rosidl_typesupport_c__typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_QuaternionToEuler_Event_message_typesupport_map),
  rosidl_typesupport_c__get_message_typesupport_handle_function,
  &arduinobot_msgs__srv__QuaternionToEuler_Event__get_type_hash,
  &arduinobot_msgs__srv__QuaternionToEuler_Event__get_type_description,
  &arduinobot_msgs__srv__QuaternionToEuler_Event__get_type_description_sources,
};

}  // namespace rosidl_typesupport_c

}  // namespace srv

}  // namespace arduinobot_msgs

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_c, arduinobot_msgs, srv, QuaternionToEuler_Event)() {
  return &::arduinobot_msgs::srv::rosidl_typesupport_c::QuaternionToEuler_Event_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "cstddef"
#include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "arduinobot_msgs/srv/detail/quaternion_to_euler__type_support.h"
// already included above
// #include "rosidl_typesupport_c/identifier.h"
#include "rosidl_typesupport_c/service_type_support_dispatch.h"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"
#include "service_msgs/msg/service_event_info.h"
#include "builtin_interfaces/msg/time.h"

namespace arduinobot_msgs
{

namespace srv
{

namespace rosidl_typesupport_c
{
typedef struct _QuaternionToEuler_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _QuaternionToEuler_type_support_ids_t;

static const _QuaternionToEuler_type_support_ids_t _QuaternionToEuler_service_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_c",  // ::rosidl_typesupport_fastrtps_c::typesupport_identifier,
    "rosidl_typesupport_introspection_c",  // ::rosidl_typesupport_introspection_c::typesupport_identifier,
  }
};

typedef struct _QuaternionToEuler_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _QuaternionToEuler_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _QuaternionToEuler_type_support_symbol_names_t _QuaternionToEuler_service_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, arduinobot_msgs, srv, QuaternionToEuler)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_c, arduinobot_msgs, srv, QuaternionToEuler)),
  }
};

typedef struct _QuaternionToEuler_type_support_data_t
{
  void * data[2];
} _QuaternionToEuler_type_support_data_t;

static _QuaternionToEuler_type_support_data_t _QuaternionToEuler_service_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _QuaternionToEuler_service_typesupport_map = {
  2,
  "arduinobot_msgs",
  &_QuaternionToEuler_service_typesupport_ids.typesupport_identifier[0],
  &_QuaternionToEuler_service_typesupport_symbol_names.symbol_name[0],
  &_QuaternionToEuler_service_typesupport_data.data[0],
};

static const rosidl_service_type_support_t QuaternionToEuler_service_type_support_handle = {
  rosidl_typesupport_c__typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_QuaternionToEuler_service_typesupport_map),
  rosidl_typesupport_c__get_service_typesupport_handle_function,
  &QuaternionToEuler_Request_message_type_support_handle,
  &QuaternionToEuler_Response_message_type_support_handle,
  &QuaternionToEuler_Event_message_type_support_handle,
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_CREATE_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    arduinobot_msgs,
    srv,
    QuaternionToEuler
  ),
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_DESTROY_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    arduinobot_msgs,
    srv,
    QuaternionToEuler
  ),
  &arduinobot_msgs__srv__QuaternionToEuler__get_type_hash,
  &arduinobot_msgs__srv__QuaternionToEuler__get_type_description,
  &arduinobot_msgs__srv__QuaternionToEuler__get_type_description_sources,
};

}  // namespace rosidl_typesupport_c

}  // namespace srv

}  // namespace arduinobot_msgs

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_c, arduinobot_msgs, srv, QuaternionToEuler)() {
  return &::arduinobot_msgs::srv::rosidl_typesupport_c::QuaternionToEuler_service_type_support_handle;
}

#ifdef __cplusplus
}
#endif