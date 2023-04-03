#pragma once

#ifndef NDEBUG
	#include <cstdlib>
	#include <iostream>

	#define ASSERT(condition, message) do { \
		if (!(condition)) {\
			std::cerr \
				<< "\nAssertion failed:" << std::endl \
				<< "   " << (message) << std::endl \
				<< "   @ " << __FILE__ << ":" << __LINE__ << std::endl; \
			\
			std::abort(); \
		} \
	} while (false)
#else
	#define ASSERT(condition, message)
#endif
