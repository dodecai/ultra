#type vertex
#version 450 core

layout (location = 0) in vec3 aPosition;

layout(std140, binding = 0) uniform Camera {
    mat4 uModel;
    mat4 uView;
    mat4 uProjection;
};

void main() {
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 0.28, 0.26, 1.0);
}
