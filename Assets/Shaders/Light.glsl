#type vertex
#version 450 core
#include <Buffers.glslh>

layout (location = 0) in vec3 aPosition;

layout (location = 0) out vec4 vColor;

void main() {
    vColor = uEntity.Color;
    gl_Position = uCamera.ViewProjection * uEntity.Transform * vec4(aPosition, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) in vec4 vColor;

layout (location = 0) out vec4 oColor;

void main() {
    oColor = vColor;
    oColor.a *= 0.2f;
}
