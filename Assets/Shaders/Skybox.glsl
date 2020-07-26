#type vertex
#version 330 core

layout (location = 0) in vec3 aPosition;

out vec3 oTextureCoordinates;

uniform mat4 uInverse;


void main() {
    vec4 position = vec4(aPosition.xy, 1.0, 1.0);
    gl_Position = position;

    oTextureCoordinates = position.xyz;
}  

#type fragment
#version 330 core

layout (location = 0) out vec4 oColor;

in vec3 aTextureCoordinates;

uniform samplerCube uSkybox;
uniform float uBlur;


void main() {
    oColor = textureLod(uSkybox, aTextureCoordinates, uBlur);
}
