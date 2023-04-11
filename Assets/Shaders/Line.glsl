// Basic Line Shader
#type vertex
#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : enable

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;

layout(location = 0) out vec4 oColor;

uniform mat4 uViewProjection;

void main() {
	oColor = aColor;
	gl_Position = uViewProjection * vec4(aPosition, 1.0);
}

#type fragment
#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : enable

layout(location = 0) out vec4 color;

layout(location = 0) in vec4 aColor;

void main() {
	color = aColor;
}