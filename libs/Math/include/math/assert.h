#pragma once

#ifndef NDEBUG
	#include <cstdlib>
	#include <iostream>

	#include <fmt/format.h>

	#define ASSERT(condition, fmt_str, ...) do { \
		if (!(condition)) {\
			std::cerr \
				<< "\nAssertion failed:" << std::endl \
				<< "   " << ::fmt::format((fmt_str), __VA_ARGS__) << std::endl \
				<< "   @ " << __FILE__ << ":" << __LINE__ << std::endl; \
			\
			std::abort(); \
		} \
	} while (false)
#else
	#define ASSERT(condition, message)
#endif
