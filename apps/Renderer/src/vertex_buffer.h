#pragma once

#include <sized.h>

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
