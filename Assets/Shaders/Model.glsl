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

layout(binding = 0) uniform sampler2D uTextureDiffuse1;
layout(binding = 1) uniform sampler2D uTextureNormal1;
layout(binding = 2) uniform sampler2D uTextureSpecular1;
layout(binding = 3) uniform sampler2D uTextureHeight1;

void main() {
    //vec4 textureColor = texture(uTextureDiffuse1, vTexCoords);
    //vec4 textureColor = vec4(1.0, 1.0, 1.0, 1.0);
    //vec4 ambientColor = vec4(0.5, 0.5, 0.5, 1.0);
    //oFragColor = ambientColor * textureColor;
    // Fixed lighting until I have time to implement it
    vec3 u_LightPosition = vec3(12.0, 24.0, 12.0);  
    vec3 u_LightColor = vec3(1.0, 1.0, 1.0);     
    vec3 u_ViewPosition = vec3(0.0, 0.0, 5.0);   
    
    // Ambient Beleuchtung
    vec3 ambient = vec3(0.5, 0.5, 0.5);  

    // Diffuse Beleuchtung
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(u_LightPosition - vFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_LightColor;

    // Specular Beleuchtung
    vec3 viewDir = normalize(u_ViewPosition - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);
    vec3 specular = spec * u_LightColor;

    vec3 result = (ambient + diffuse + specular);
    oFragColor = vec4(result, 1.0);
}
