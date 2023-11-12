// Model Shaders
#type vertex
#version 450 core
#include <Buffers.glslh>
#include <Common.glslh>
#include <Constants.glslh>

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

void main() {
    vTexCoords = aTexCoords;
    vNormal = mat3(transpose(inverse(uEntity.Transform))) * aNormal;
    vFragPos = vec3(uEntity.Transform * vec4(aPosition, 1.0));

    gl_Position = uCamera.ViewProjection * uEntity.Transform * vec4(aPosition, 1.0);
}

#type fragment
#version 450 core
#extension GL_EXT_scalar_block_layout : require
#include <Buffers.glslh>
#include <Common.glslh>
#include <Constants.glslh>

layout (location = 0) out vec4 oFragColor;

layout (location = 0) in vec2 vTexCoords;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vFragPos;

layout(binding = 0) uniform sampler2D uTextureDiffuse;
layout(binding = 1) uniform sampler2D uTextureNormal;
layout(binding = 2) uniform sampler2D uTextureSpecular;
layout(binding = 3) uniform sampler2D uTextureHeight;

vec4 CalculateDirectionalLight(Light light, vec3 normal, vec3 viewDirection, bool materialActive);
vec4 CalculatePointLight(Light light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection, bool materialActive);
vec4 CalculateSpotLight(Light light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection, bool materialActive);

void main() {
    // Properties
    vec4 color = vec4(0.0f);
    vec3 normal = normalize(vNormal);
    vec3 viewDirection = normalize(uCamera.Position  - vFragPos);

    // Phong Lighting
    bool materialActive = all(equal(uMaterial.AmbientColor, vec3(0.0f))) && all(equal(uMaterial.DiffuseColor, vec3(0.0f))) && all(equal(uMaterial.SpecularColor, vec3(0.0f)));
    materialActive = true;
    uint count = uLightCount;
    for (uint i = 0; i < count; i++) {
        if (uLights[i].Type == DIRECTIONAL_LIGHT) {
            // Phase 1: Directional Lighting
            color += CalculateDirectionalLight(uLights[i], normal, viewDirection, materialActive);
        } else if (uLights[i].Type == POINT_LIGHT) {
            // Phase 2: Point Lighting
            color += CalculatePointLight(uLights[i], normal, vFragPos, viewDirection, materialActive);
        } else if (uLights[i].Type == SPOT_LIGHT) {
            // Phase 3: Spotlight
            color += CalculateSpotLight(uLights[i], normal, vFragPos, viewDirection, materialActive);
        }
    }

    float alpha = texture(uTextureDiffuse, vTexCoords).a;
    if (alpha < EPSILON) discard;
    oFragColor = vec4(color.rgb, alpha);
}


vec4 CalculateDirectionalLight(Light light, vec3 normal, vec3 viewDirection, bool materialActive) {
    // Properties
    vec3 color = vec3(0.0f);
    vec3 lightDirection = normalize(-light.Direction);
    // Diffuse Shading
    float diffuse = max(dot(normal, lightDirection), 0.0f);
    // Specular Shading
    float shininess = uMaterial.Shininess;
    if (shininess == 0.0f) shininess = 1.0f;
    const float energyConservation = ( 8.0 + shininess) / ( 8.0 * PI ); 
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    float specular = energyConservation * pow(max(dot(normal, halfwayDirection), 0.0f), shininess);
    //float specular = max(dot(normal, halfwayDirection), 0.0f);
    
    // Ambient lighting
    if (materialActive) {
        color += light.Ambient * texture(uTextureDiffuse, vTexCoords).rgb;
    } else {
        color += light.Ambient * uMaterial.AmbientColor;
    }
    
    // Diffuse lighting
    if (materialActive) {
        color += light.Diffuse * diffuse * texture(uTextureDiffuse, vTexCoords).rgb;
    } else {
        color += light.Diffuse * (diffuse * uMaterial.DiffuseColor);
    }
    
    // Specular lighting
    if (materialActive) {
        color += light.Specular * specular * texture(uTextureSpecular, vTexCoords).rgb;
    } else {
        color += light.Specular * (specular * uMaterial.SpecularColor);
    }
    
    return vec4(color, 1.0f);
}

vec4 CalculatePointLight(Light light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection, bool materialActive) {    
    // Properties
    vec3 color = vec3(0.0f);
    vec3 lightDirection = normalize(light.Position - vFragPos);
    // Diffuse Shading
    float diffuse = max(dot(normal, lightDirection), 0.0f);
    // Specular Shading
    float shininess = uMaterial.Shininess;
    if (shininess == 0.0f) shininess = 1.0f;
    const float energyConservation = ( 8.0 + shininess) / ( 8.0 * PI ); 
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    float specular = energyConservation * pow(max(dot(normal, halfwayDirection), 0.0f), shininess);
    //float specular = max(dot(normal, halfwayDirection), 0.0f);
    // Attenuation
    float distance = length(light.Position - vFragPos);
    float attenuation = 1.0f / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));
    
    // Ambient lighting
    if (materialActive) {
        color += light.Ambient * texture(uTextureDiffuse, vTexCoords).rgb;
    } else {
        color += (light.Ambient * light.Color) * uMaterial.AmbientColor;
    }
    color *= attenuation;
    
    // Diffuse lighting
    if (materialActive) {
        color += light.Diffuse * diffuse * texture(uTextureDiffuse, vTexCoords).rgb;
    } else {
        color += (light.Diffuse * light.Color) * (diffuse * uMaterial.DiffuseColor);
    }
    color *= attenuation;

    // Specular lighting
    if (materialActive) {
        color += light.Specular * specular * texture(uTextureSpecular, vTexCoords).rgb;
    } else {
        color += (light.Specular * light.Color) * (specular * uMaterial.SpecularColor);
    }
    color *= attenuation;

    return vec4(color, 1.0f);
}

vec4 CalculateSpotLight(Light light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection, bool materialActive) {
    // Properties
    vec3 color = vec3(0.0f);
    vec3 lightDirection = normalize(light.Position - vFragPos);
    // Diffuse Shading
    float diffuse = max(dot(normal, lightDirection), 0.0f);
    // Specular Shading
    float shininess = uMaterial.Shininess;
    if (shininess == 0.0f) shininess = 1.0f;
    const float energyConservation = ( 8.0 + shininess) / ( 8.0 * PI ); 
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    float specular = energyConservation * pow(max(dot(normal, halfwayDirection), 0.0f), shininess);
    //float specular = max(dot(normal, halfwayDirection), 0.0f);
    // Attenuation
    float distance = length(light.Position - vFragPos);
    float attenuation = 1.0f / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));
    // Intensity
    float theta = dot(lightDirection, normalize(-light.Direction));
    float epsilon = light.CuttOffAngle - (light.CuttOffAngle - 0.05f);
    float intensity = clamp((theta - (light.CuttOffAngle - 0.05f)) / epsilon, 0.0f, 1.0f);
        
    // Ambient lighting
    if (materialActive) {
        color += light.Ambient * texture(uTextureDiffuse, vTexCoords).rgb;
    } else {
        color += (light.Ambient * light.Color) * uMaterial.AmbientColor;
    }
    color *= attenuation * intensity;
    
    // Diffuse lighting
    if (materialActive) {
        color += light.Diffuse * diffuse * texture(uTextureDiffuse, vTexCoords).rgb;
    } else {
        color += (light.Diffuse * light.Color) * (diffuse * uMaterial.DiffuseColor);
    }
    color *= attenuation * intensity;

    // Specular lighting
    if (materialActive) {
        color += light.Specular * specular * texture(uTextureSpecular, vTexCoords).rgb;
    } else {
        color += (light.Specular * light.Color) * (specular * uMaterial.SpecularColor);
    }
    color *= attenuation * intensity;

    return vec4(color, 1.0f);
}
