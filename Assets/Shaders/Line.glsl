// Basic Line Shader
#type vertex
#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 0) out vec4 vColor;

layout(std140, binding = 0) uniform Camera {
    mat4 uViewProjection;
    mat4 uView;
    mat4 uProjection;
    float uNearClip;
    float uFarClip;
};

void main() {
	vColor = aColor;
	gl_Position = uViewProjection * vec4(aPosition, 1.0);
}

#type fragment
#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 vColor;
layout(location = 0) out vec4 oColor;

void main() {
	oColor = vColor;
}