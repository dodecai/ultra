#type vertex
#version 450 core
#include <Buffers.glslh>

layout (location = 0) in vec3 aPosition;

void main() {
    gl_Position = uCamera.ViewProjection * uEntity.Transform * vec4(aPosition, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 0.28, 0.26, 1.0);
}
