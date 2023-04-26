#pragma once

#include <initializer_list>
#include <vector>

#include <sized.h>

#include "api/gl/types.h"

using namespace sized;


class VertexBuffer {
public:
	VertexBuffer(const void* data, u32 size);
	~VertexBuffer();

	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;

	VertexBuffer(VertexBuffer&&) noexcept = default;
	VertexBuffer& operator=(VertexBuffer&&) noexcept = default;

	void bind() const;
	void unbind() const;

private:
	u32 m_renderer_id = 0;
};


struct VertexBufferElement {
	/**
	 * Specifies the data type of each component in the array. The initial value
	 * is `Scalar::Float`.
	 */
	gl::Scalar type = gl::Scalar::Float;
	/**
	 * Specifies the number of components per generic vertex attribute. Must be
	 * 1, 2, 3, 4, or `VertexAttribParams::k_BGRA`. The initial value is 4.
	 */
	usize count = 4;
	/**
	 * specifies whether fixed-point data values should be normalized (`true`)
	 * or converted directly as fixed-point values (`false`) when they are
	 * accessed.
	 */
	bool normalized = false;
};

class VertexBufferLayout {
public:
	VertexBufferLayout() = default;
	VertexBufferLayout(std::initializer_list<VertexBufferElement> elements);

	auto stride() const -> usize { return m_stride; }
	auto elements() const -> const std::vector<VertexBufferElement>& { return m_elements; }

	template <typename T>
	void push(usize count, bool normalized = false);

private:
	usize m_stride = 0;
	std::vector<VertexBufferElement> m_elements;
};


template <typename T>
inline void VertexBufferLayout::push(usize count, bool normalized)
{
	m_elements.push_back({
		.type = gl::type<T>(),
		.count = count,
		.normalized = normalized,
	});

	m_stride += count * sizeof(T);
}
