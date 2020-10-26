#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : enable

layout(location = 0) in vec4 aColor;

layout(location = 0) out vec4 oColor;

void main() {
    oColor = aColor;
}
