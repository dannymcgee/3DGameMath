#pragma once

#include <type_traits>

#define ENABLE_IF(condition, T) \
	typename std::enable_if<condition, T>::type

#define IS_SAME(T1, T2) \
	std::is_same_v<typename T1, T2>

#define ITER_OF(Iter, Type) \
	std::is_same_v<typename Iter::value_type, Type>
