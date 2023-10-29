#type vertex
#version 450 core

layout(location = 0) in vec3 aPosition;

layout(location = 0) out vec3 vTextureCoordinates;

layout(std140, binding = 0) uniform Camera {
    mat4 uModel;
    mat4 uView;
    mat4 uProjection;
};

void main() {
    vTextureCoordinates = aPosition;

    vec4 position = uProjection * uView * vec4(aPosition, 1.0);
    gl_Position = position.xyww;
}

#type fragment
#version 450 core

layout(location = 0) in vec3 vTextureCoordinates;

layout(location = 0) out vec4 FragColor;

layout(binding=0) uniform samplerCube uCubemap;

void main() {
    FragColor = texture(uCubemap, vTextureCoordinates);
}
