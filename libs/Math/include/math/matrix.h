#pragma once

#include <exception>
#include <initializer_list>
#include <sized.h>

#include "fmt/format.h"
#include "math/vector.h"

namespace math {

using namespace sized; // NOLINT(*-using-namespace)

template <usize Rows, usize Cols, typename T = f64>
class Mat {
public:
	Mat() = default;
	Mat(std::initializer_list<Vec<Cols, T>> rows);

private:
	std::array<Vec<Cols, T>, Rows> m_data; // NOLINT(*-avoid-c-arrays)
};


template <usize Rows, usize Cols, typename T>
Mat<Rows,Cols,T>::Mat(std::initializer_list<Vec<Cols,T>> rows)
{
	if (rows.size() > Rows) {
		auto err_msg = fmt::format(
			"Too many arguments for Mat<{},{}>: Expected {}, received {}",
			Rows, Cols,
			Rows, rows.size()
		);

		throw std::exception{ err_msg.c_str(), 1 };
	}

	usize i = 0;
	for (auto row : rows) {
		m_data[i] = row;
		++i;
	}
}

}
