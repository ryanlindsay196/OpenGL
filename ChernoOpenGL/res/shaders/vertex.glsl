#version 330

layout (location = 0) in vec3 vertex_position;
//layout (location = 1) in vec3 vertex_color;
layout (location = 1) in vec2 vertex_texcoord;
layout (location = 2) in vec3 vertex_normal;

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_texcoord;
out vec3 vs_normal;

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform vec3 scale;

//out vec3 tempPosition;

void main()
{
	vs_position = vec4(M * vec4(vertex_position, 1.f)).xyz;
	//vs_color = vertex_color;
	vs_texcoord = vec2(vertex_texcoord.x, vertex_texcoord.y * -1.f);
	vs_normal = mat3(M) * vertex_normal;
	//tempPosition = vec4(MVP * vec4(vertex_position, 1.0f)).xyz;

	gl_Position = MVP * vec4(vertex_position * scale, 1.f);
	//gl_Position = MVP * vec4(vertex_position * vec3(1.f, 0.3f, 1.f), 1.f);
	//gl_Position = M * vec4(vertex_position, 1.f);
}