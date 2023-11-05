#type vertex
#version 450 core
#include <Buffers.glslh>

layout(location = 0) in vec3 aPosition;

layout(location = 0) out vec3 vTextureCoordinates;

void main() {
    vTextureCoordinates = aPosition;

    vec4 position = uCamera.Projection * mat4(mat3(uCamera.View)) * vec4(aPosition, 1.0);
    gl_Position = position.xyww;
}

#type fragment
#version 450 core
//#include <Common.glslh>

layout(location = 0) in vec3 vTextureCoordinates;

layout(location = 0) out vec4 FragColor;

layout(binding=0) uniform samplerCube uCubemap;

void main() {
    FragColor = texture(uCubemap, vTextureCoordinates);
}
