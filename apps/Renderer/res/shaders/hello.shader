#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 vert_color;

out vec4 frag_color;

void main()
{
	gl_Position = position;
	frag_color = vert_color;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 frag_color;

uniform vec4 u_color;

void main()
{
	color = u_color; // frag_color;
}
