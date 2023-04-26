#include "vertex_array.h"

#include "api/gl/gl.h"
#include "vertex_buffer.h"


VertexArray::VertexArray()
	: m_renderer_id(gl::gen_vertex_array())
{}

VertexArray::~VertexArray()
{
	gl::delete_vertex_array(m_renderer_id);
}

void VertexArray::add_buffer(const VertexBuffer &buffer, const VertexBufferLayout &layout) const
{
	bind();
	buffer.bind();

	usize idx = 0;
	usize offset = 0;
	for (const auto& element : layout.elements()) {
		gl::enable_vertex_attrib_array(idx);
		gl::vertex_attrib_pointer(idx, {
			.type = element.type,
			.size = static_cast<i32>(element.count),
			.normalized = element.normalized,
			.stride = static_cast<i32>(layout.stride()),
			.offset = reinterpret_cast<const void*>(offset), // NOLINT
		});

		++idx;
		offset += element.count * gl::size_of(element.type);
	}
}

void VertexArray::bind() const
{
	gl::bind_vertex_array(m_renderer_id);
}

void VertexArray::unbind() const
{
	gl::bind_vertex_array(0);
}
