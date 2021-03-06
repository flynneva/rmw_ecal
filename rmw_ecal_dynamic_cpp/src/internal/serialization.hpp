/* ========================= RMW eCAL LICENSE =================================
 *
 * Copyright (C) 2019 - 2020 Continental Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * ========================= RMW eCAL LICENSE =================================
*/

#pragma once

#include <rosidl_typesupport_introspection_cpp/message_introspection.hpp>
#include <rosidl_typesupport_introspection_c/message_introspection.h>

#include "internal/serialization/custom/serializer_cpp.hpp"
#include "internal/serialization/custom/serializer_c.hpp"
#include "internal/serialization/custom/deserializer_cpp.hpp"
#include "internal/serialization/custom/deserializer_c.hpp"

#include "internal/serialization/protobuf/serializer_cpp.hpp"
#include "internal/serialization/protobuf/serializer_c.hpp"
#include "internal/serialization/protobuf/deserializer_cpp.hpp"
#include "internal/serialization/protobuf/deserializer_c.hpp"

namespace eCAL
{
namespace rmw
{

static constexpr auto serialization_format =
#ifdef USE_PROTOBUF_SERIALIZATION
	"protobuf";
#else
	"custom";
#endif

static const std::string serialization_typename_prefix =
#ifdef USE_PROTOBUF_SERIALIZATION
	"proto:";
#else
	"";
#endif

inline Serializer *CreateSerializer(const rosidl_typesupport_introspection_cpp::MessageMembers *members)
{
#ifdef USE_PROTOBUF_SERIALIZATION
	return new CppProtobufSerializer(members);
#else
	return new CppSerializer(members);
#endif
}

inline Serializer *CreateSerializer(const rosidl_typesupport_introspection_c__MessageMembers *members)
{
#ifdef USE_PROTOBUF_SERIALIZATION
	return new CProtobufSerializer(members);
#else
	return new CSerializer(members);
#endif
}

inline Deserializer *CreateDeserializer(const rosidl_typesupport_introspection_cpp::MessageMembers *members)
{
#ifdef USE_PROTOBUF_SERIALIZATION 
	return new CppProtobufDeserializer(members);
#else
	return new CppDeserializer(members);
#endif
}

inline Deserializer *CreateDeserializer(const rosidl_typesupport_introspection_c__MessageMembers *members)
{
#ifdef USE_PROTOBUF_SERIALIZATION
	return new CProtobufDeserializer(members);
#else
	return new CDeserializer(members);
#endif
}

inline Serializer *CreateSerializer(const rosidl_message_type_support_t *type_support)
{
	auto ts = get_message_typesupport_handle(type_support, rosidl_typesupport_introspection_cpp::typesupport_identifier);
	if (ts != nullptr)
	{
		auto members = GetCppMembers(ts);
		return CreateSerializer(members);
	}

	ts = get_message_typesupport_handle(type_support, rosidl_typesupport_introspection_c__identifier);
	if (ts != nullptr)
	{
		auto members = GetCMembers(ts);
		return CreateSerializer(members);
	}
	throw std::runtime_error("Unsupported type support.");
}

inline Deserializer *CreateDeserializer(const rosidl_message_type_support_t *type_support)
{
	auto ts = get_message_typesupport_handle(type_support, rosidl_typesupport_introspection_cpp::typesupport_identifier);
	if (ts != nullptr)
	{
		auto members = GetCppMembers(ts);
		return CreateDeserializer(members);
	}

	ts = get_message_typesupport_handle(type_support, rosidl_typesupport_introspection_c__identifier);
	if (ts != nullptr)
	{
		auto members = GetCMembers(ts);
		return CreateDeserializer(members);
	}
	throw std::runtime_error("Unsupported type support.");
}

} // namespace rmw
} // namespace eCAL
