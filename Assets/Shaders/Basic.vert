#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : enable

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;

layout(std140, binding = 0) uniform Camera {
    mat4 uViewProjection;
};
layout(push_constant, binding = 1) uniform VertexUniforms {
    mat4 uTransform;
};

layout(location = 0) out vec4 oColor;

void main() {
	oColor = aColor;
    gl_Position = vec4(aPosition, 1.0);
}
