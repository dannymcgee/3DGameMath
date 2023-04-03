#pragma once


#define _IMPL_BITFLAGS_CTORS(Class, Flags, type) \
	Class(Flags value) : m_value(value) {} \
	Class(type value) : m_value(static_cast<Flags>(value)) {}

#define _IMPL_BITFLAGS_BINOP(Class, Flags, type, op) \
	auto operator op (Class other) const -> Class { \
		return static_cast<type>(m_value) op static_cast<type>(other); \
	} \
	auto operator op (type other) const -> Class { \
		return static_cast<type>(m_value) op other; \
	}

#define _IMPL_BITFLAGS_ASSIGNOP(Class, Flags, type, opeq, binop) \
	auto operator opeq (Class other) -> Class& { \
		m_value = static_cast<Flags>(static_cast<type>(m_value) binop static_cast<type>(other)); \
		return *this; \
	} \
	auto operator opeq (type other) -> Class& { \
		m_value = static_cast<Flags>(static_cast<type>(m_value) binop other); \
		return *this; \
	}

#define _IMPL_BITFLAGS_UNARYOP(Class, Flags, type, op) \
	auto operator op () const -> Class { \
		return op static_cast<type>(m_value); \
	}

#define IMPL_BITFLAGS(Class, Flags, type) \
/* NOLINTBEGIN */ \
public: \
	_IMPL_BITFLAGS_CTORS(Class, Flags, type) \
	\
	operator bool() { return static_cast<type>(m_value) != 0; } \
	\
	_IMPL_BITFLAGS_BINOP(Class, Flags, type, |) \
	_IMPL_BITFLAGS_ASSIGNOP(Class, Flags, type, |=, |) \
	\
	_IMPL_BITFLAGS_BINOP(Class, Flags, type, &) \
	_IMPL_BITFLAGS_ASSIGNOP(Class, Flags, type, &=, &) \
	\
	_IMPL_BITFLAGS_BINOP(Class, Flags, type, ^) \
	_IMPL_BITFLAGS_ASSIGNOP(Class, Flags, type, ^=, ^) \
	\
	_IMPL_BITFLAGS_UNARYOP(Class, Flags, type, ~) \
	\
private: \
	Flags m_value; \
/* NOLINTEND */
