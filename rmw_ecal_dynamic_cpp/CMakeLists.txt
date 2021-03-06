# ========================= RMW eCAL LICENSE =================================
#
# Copyright (C) 2019 - 2020 Continental Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#      http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# ========================= RMW eCAL LICENSE =================================

cmake_minimum_required(VERSION 3.12)

project(rmw_ecal_dynamic_cpp)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	add_compile_options(/W4)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

if(NOT $ENV{ROS_DISTRO} MATCHES "dashing" AND NOT $ENV{ROS_DISTRO} MATCHES "eloquent" AND NOT $ENV{ROS_DISTRO} MATCHES "foxy")
	message(FATAL_ERROR "'$ENV{ROS_DISTRO}' is unsupported ros2 distro.")
endif()

add_compile_definitions("DASHING=0")
add_compile_definitions("ELOQUENT=1")
add_compile_definitions("FOXY=2")

add_compile_definitions("ROS_DISTRO=$<UPPER_CASE:$ENV{ROS_DISTRO}>")

option(USE_PROTOBUF_SERIALIZATION "Use google protobuf as serialization format." OFF)
if(USE_PROTOBUF_SERIALIZATION)
	add_compile_definitions(USE_PROTOBUF_SERIALIZATION)
endif()

find_package(rmw REQUIRED)
find_package(rosidl_generator_c REQUIRED)
find_package(rosidl_typesupport_introspection_c REQUIRED)
find_package(rosidl_typesupport_introspection_cpp REQUIRED)

find_package(ament_cmake REQUIRED)

find_package(eCAL REQUIRED)
find_package(Protobuf REQUIRED)

create_targets_protobuf()

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

include_directories(
	${PROJECT_SOURCE_DIR}/src
)

add_library(${PROJECT_NAME} SHARED
	src/event.cpp
	src/get_node_info_and_types.cpp
	src/get_service_names_and_types.cpp
	src/get_topic_names_and_types.cpp
	src/get_topic_endpoint_info.cpp
	src/init_options.cpp
	src/init.cpp
	src/rmw.cpp
	src/internal/serialization/custom/serializer_cpp.cpp
	src/internal/serialization/custom/serializer_c.cpp
	src/internal/serialization/custom/deserializer_cpp.cpp
	src/internal/serialization/custom/deserializer_c.cpp
	src/internal/serialization/protobuf/serializer_cpp.cpp
	src/internal/serialization/protobuf/serializer_c.cpp
	src/internal/serialization/protobuf/deserializer_cpp.cpp
	src/internal/serialization/protobuf/deserializer_c.cpp
)

set(proto_files
	${CMAKE_CURRENT_SOURCE_DIR}/protobuf/subscriber.proto
	${CMAKE_CURRENT_SOURCE_DIR}/protobuf/publisher.proto
	${CMAKE_CURRENT_SOURCE_DIR}/protobuf/client.proto
	${CMAKE_CURRENT_SOURCE_DIR}/protobuf/service.proto
)

PROTOBUF_TARGET_CPP(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/protobuf ${proto_files})

target_link_libraries(${PROJECT_NAME} 
	eCAL::core 
	eCAL::pb
)

ament_target_dependencies(${PROJECT_NAME} 
	rmw 
	rosidl_typesupport_introspection_cpp
)

configure_rmw_library(${PROJECT_NAME})

ament_export_libraries(${PROJECT_NAME})

install(
	TARGETS ${PROJECT_NAME}
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin
)

register_rmw_implementation(
	"c:rosidl_typesupport_c:rosidl_typesupport_introspection_c"
	"cpp:rosidl_typesupport_cpp:rosidl_typesupport_introspection_cpp"
)

ament_package()
