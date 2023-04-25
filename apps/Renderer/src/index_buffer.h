#pragma once

#include <sized.h>

using namespace sized;


class IndexBuffer {
public:
	IndexBuffer(const u32 data[], u32 count);
	~IndexBuffer();

	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;

	IndexBuffer(IndexBuffer&&) noexcept = default;
	IndexBuffer& operator=(IndexBuffer&&) noexcept = default;

	auto count() const -> u32 { return m_count; }

	void bind() const;
	void unbind() const;

private:
	u32 m_renderer_id = 0;
	u32 m_count = 0;
};
