// Basic Shader
#type vertex
#version 450 core
#include <Buffers.glslh>

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

layout (location = 0) out vec4 vColor;
layout (location = 1) out vec2 vTexCoord;

void main() {
    vColor = aColor;
    vTexCoord = aTexCoord;

    gl_Position = uCamera.ViewProjection * uEntity.Transform * vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
}

#type fragment
#version 450 core
#include <Buffers.glslh>

layout (location = 0) in vec4 vColor;
layout (location = 1) in vec2 vTexCoord;
        
layout (location = 0) out vec4 oFragColor;

layout(binding = 1) uniform sampler2D uTexture;

void main() {
    // mix(a, b, percent from a)
    oFragColor = texture(uTexture, vTexCoord) * uEntity.Color;// * vColor;
}
