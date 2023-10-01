// Model Shaders
#type vertex
#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
//layout (location = 5) in ivec4 aBoneIDs;
//layout (location = 6) in vec4 aWeights;

layout (location = 0) out vec2 vTexCoords;
layout (location = 1) out vec3 vNormal;
layout (location = 2) out vec3 vFragPos;

layout(std140, binding = 0) uniform Camera {
    mat4 uModel;
    mat4 uView;
    mat4 uProjection;
};

void main() {
    vTexCoords = aTexCoords;
    vNormal = mat3(uModel) * aNormal;
    vFragPos = vec3(uModel * vec4(aPosition, 1.0));
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) in vec2 vTexCoords;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vFragPos;

layout (location = 0) out vec4 oFragColor;

layout(binding = 0) uniform sampler2D uTextureDiffuse;
layout(binding = 1) uniform sampler2D uTextureNormal;
layout(binding = 2) uniform sampler2D uTextureSpecular;
layout(binding = 3) uniform sampler2D uTextureHeight;

layout(std140, binding = 4) uniform Material {
    float uShininess;
};

layout(std140, binding = 5) uniform Light {
    vec3 uLightPosition;
    vec3 uAmbient;
    vec3 uDiffuse;
    vec3 uSpecular;
    bool uAmbientEnabled;
    bool uDiffuseEnabled;
    bool uSpecularEnabled;
};

layout(std140, binding = 6) uniform View {
    vec3 uViewPosition;
};

void main() {
    // Preparation
    vec3 result = vec3(0.0f);
    vec3 direction = normalize(uLightPosition - vFragPos);
    vec3 normal = normalize(vNormal);
    vec3 viewDirection = normalize(uViewPosition  - vFragPos);
    vec3 reflectDirection = reflect(-direction, normal);

    // Ambient lighting
    if (uAmbientEnabled) {
    }
        result += uAmbient * texture(uTextureDiffuse, vTexCoords).rgb;
    
    // Diffuse lighting
    if (uDiffuseEnabled) {
    }
        float diff = max(dot(normal, direction), 0.0f);
        result += diff * uDiffuse * texture(uTextureDiffuse, vTexCoords).rgb;

    // Specular lighting
    if (uSpecularEnabled) {
    }
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0),uShininess);
    result += spec * uSpecular * texture(uTextureSpecular, vTexCoords).rgb;

    oFragColor = vec4(result, 1.0);
    oFragColor.a = 1.0f;
}
