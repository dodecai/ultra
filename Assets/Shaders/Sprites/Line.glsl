// Basic Line Shader
#type vertex
#version 450 core
#extension GL_ARB_separate_shader_objects : enable
#include <Buffers.glslh>

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;

layout(location = 0) out vec4 vColor;

void main() {
	vColor = aColor;
	gl_Position = uCamera.ViewProjection * vec4(aPosition, 1.0);
}

#type fragment
#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 vColor;

layout(location = 0) out vec4 oColor;

void main() {
	oColor = vColor;
}