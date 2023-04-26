#include "vertex_buffer.h"

#include "api/gl/gl.h"


VertexBuffer::VertexBuffer(const void* data, u32 size)
	: m_renderer_id(gl::gen_buffer())
{
	gl::bind_buffer(gl::Target::Array, m_renderer_id);
	gl::buffer_data(gl::Target::Array, static_cast<i32>(size), data, gl::Usage::StaticDraw);
}

VertexBuffer::~VertexBuffer()
{
	gl::delete_buffer(m_renderer_id);
}

void VertexBuffer::bind() const
{
	gl::bind_buffer(gl::Target::Array, m_renderer_id);
}

void VertexBuffer::unbind() const
{
	gl::bind_buffer(gl::Target::Array, 0);
}


VertexBufferLayout::VertexBufferLayout(std::initializer_list<VertexBufferElement> elements)
{
	for (const auto& element : elements) {
		m_elements.push_back(element);
		m_stride += element.count * gl::size_of(element.type);
	}
}
