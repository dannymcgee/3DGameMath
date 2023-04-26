#pragma once

#include <sized.h>

using namespace sized;

class VertexBuffer;
class VertexBufferLayout;


class VertexArray {
public:
	VertexArray();
	~VertexArray();

	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;

	VertexArray(VertexArray&&) noexcept = default;
	VertexArray& operator=(VertexArray&&) noexcept = default;

	void add_buffer(const VertexBuffer& buffer, const VertexBufferLayout& layout) const;

	void bind() const;
	void unbind() const;

private:
	u32 m_renderer_id;
};
