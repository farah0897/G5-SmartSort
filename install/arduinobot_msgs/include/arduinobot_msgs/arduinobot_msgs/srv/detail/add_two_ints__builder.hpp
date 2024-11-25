// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from arduinobot_msgs:srv/AddTwoInts.idl
// generated code does not contain a copyright notice

#ifndef ARDUINOBOT_MSGS__SRV__DETAIL__ADD_TWO_INTS__BUILDER_HPP_
#define ARDUINOBOT_MSGS__SRV__DETAIL__ADD_TWO_INTS__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "arduinobot_msgs/srv/detail/add_two_ints__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace arduinobot_msgs
{

namespace srv
{

namespace builder
{

class Init_AddTwoInts_Request_second
{
public:
  explicit Init_AddTwoInts_Request_second(::arduinobot_msgs::srv::AddTwoInts_Request & msg)
  : msg_(msg)
  {}
  ::arduinobot_msgs::srv::AddTwoInts_Request second(::arduinobot_msgs::srv::AddTwoInts_Request::_second_type arg)
  {
    msg_.second = std::move(arg);
    return std::move(msg_);
  }

private:
  ::arduinobot_msgs::srv::AddTwoInts_Request msg_;
};

class Init_AddTwoInts_Request_first
{
public:
  Init_AddTwoInts_Request_first()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_AddTwoInts_Request_second first(::arduinobot_msgs::srv::AddTwoInts_Request::_first_type arg)
  {
    msg_.first = std::move(arg);
    return Init_AddTwoInts_Request_second(msg_);
  }

private:
  ::arduinobot_msgs::srv::AddTwoInts_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::arduinobot_msgs::srv::AddTwoInts_Request>()
{
  return arduinobot_msgs::srv::builder::Init_AddTwoInts_Request_first();
}

}  // namespace arduinobot_msgs


namespace arduinobot_msgs
{

namespace srv
{

namespace builder
{

class Init_AddTwoInts_Response_sum
{
public:
  Init_AddTwoInts_Response_sum()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::arduinobot_msgs::srv::AddTwoInts_Response sum(::arduinobot_msgs::srv::AddTwoInts_Response::_sum_type arg)
  {
    msg_.sum = std::move(arg);
    return std::move(msg_);
  }

private:
  ::arduinobot_msgs::srv::AddTwoInts_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::arduinobot_msgs::srv::AddTwoInts_Response>()
{
  return arduinobot_msgs::srv::builder::Init_AddTwoInts_Response_sum();
}

}  // namespace arduinobot_msgs

#endif  // ARDUINOBOT_MSGS__SRV__DETAIL__ADD_TWO_INTS__BUILDER_HPP_
