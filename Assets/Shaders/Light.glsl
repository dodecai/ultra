#type vertex
#version 450 core

layout (location = 0) in vec3 aPosition;

layout (location = 0) out vec4 vColor;

layout (std140, binding = 0) uniform Properties {
    vec4 uColor;
};
layout (std140, binding = 1) uniform Translation {
    mat4 uTransform;
};

void main() {
    vColor = uColor;
    gl_Position = uTransform * vec4(aPosition, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) in vec4 vColor;

layout (location = 0) out vec4 oColor;

void main() {
    oColor = vColor;
    oColor.a *= 0.2f;
}
