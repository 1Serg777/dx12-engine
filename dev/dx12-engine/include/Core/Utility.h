#pragma once

namespace dxe
{
#define THIS_FILE __FILE__
#define THIS_FUNCTION __FUNCTION__
#define THIS_LINE __LINE__

#define NODISCARD [[nodiscard]]
#define NOEXCEPT noexcept

#define CLASS_DEFAULT_COPY(class_name)\
	class_name(const class_name& copy) = default;\
	class_name& operator=(const class_name& copy) = default

#define CLASS_DEFAULT_MOVE(class_name)\
	class_name(class_name&& move) = default;\
	class_name& operator=(class_name&& move) = default

#define CLASS_NO_COPY(class_name)\
	class_name(const class_name& copy) = delete;\
	class_name& operator=(const class_name& copy) = delete

#define CLASS_NO_MOVE(class_name)\
	class_name(class_name&& move) = delete;\
	class_name& operator=(class_name&& move) = delete

#define STRUCT_DEFAULT_COPY(struct_name) CLASS_DEFAULT_COPY(struct_name)

#define STRUCT_DEFAULT_MOVE(struct_name) CLASS_DEFAULT_MOVE(struct_name)

#define STRUCT_NO_COPY(struct_name) CLASS_NO_COPY(struct_name)

#define STRUCT_NO_MOVE(struct_name) CLASS_NO_MOVE(struct_name)
}