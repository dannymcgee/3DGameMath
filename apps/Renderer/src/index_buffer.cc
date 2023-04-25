#include "index_buffer.h"

#include "api/gl/gl.h"


IndexBuffer::IndexBuffer(const u32 data[], u32 count)
	: m_renderer_id(gl::gen_buffer())
	, m_count(count)
{
	gl::bind_buffer(gl::Target::ElementArray, m_renderer_id);
	gl::buffer_data(
		gl::Target::ElementArray,
		static_cast<i32>(count * sizeof(u32)),
		data,
		gl::Usage::StaticDraw);
}

IndexBuffer::~IndexBuffer()
{
	gl::delete_buffer(m_renderer_id);
}

void IndexBuffer::bind() const
{
	gl::bind_buffer(gl::Target::ElementArray, m_renderer_id);
}

void IndexBuffer::unbind() const
{
	gl::bind_buffer(gl::Target::ElementArray, 0);
}
