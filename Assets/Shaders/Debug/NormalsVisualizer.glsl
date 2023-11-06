#type vertex
#version 450 core
#include <Buffers.glslh>

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;

layout (location = 0) out VertexData {
    vec3 Normal;
} oVertexData;

void main() {
    mat3 normal = mat3(transpose(inverse(uCamera.View * uEntity.Transform)));
    oVertexData.Normal = normalize(normal * aNormal);

    gl_Position = uCamera.View * uEntity.Transform * vec4(aPosition, 1.0);
}

#type geometry
#version 450 core
#include <Buffers.glslh>

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

const float MAGNITUDE = 0.2f;

layout (location = 0) in VertexData {
    vec3 Normal;
} gs_in[];

void GenerateLine(int index) {
    gl_Position = uCamera.Projection * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = uCamera.Projection * (gl_in[index].gl_Position + vec4(gs_in[index].Normal, 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}


void main() {
    GenerateLine(0);
    GenerateLine(1);
    GenerateLine(2);
}  

#type fragment
#version 450 core

layout (location = 0) out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}
