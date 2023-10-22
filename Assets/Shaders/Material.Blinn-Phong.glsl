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
    vNormal = mat3(transpose(inverse(uModel))) * aNormal;
    vFragPos = vec3(uModel * vec4(aPosition, 1.0));

    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}

#type fragment
#version 450 core
#extension GL_EXT_scalar_block_layout : require

layout (location = 0) out vec4 oFragColor;

layout (location = 0) in vec2 vTexCoords;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vFragPos;

layout(binding = 0) uniform sampler2D uTextureDiffuse;
layout(binding = 1) uniform sampler2D uTextureNormal;
layout(binding = 2) uniform sampler2D uTextureSpecular;
layout(binding = 3) uniform sampler2D uTextureHeight;

layout(std140, binding = 4) uniform View {
    vec3 uViewPosition;
};

layout(std140, binding = 5) uniform Material {
    vec3 uMaterialAmbientColor;
    vec3 uMaterialDiffuseColor;
    vec3 uMaterialSpecularColor;
    float uShininess;
};

#define MAX_LIGHTS 5
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2
struct Light {
    uint Type;
    vec3 Color;
    vec3 Position;
    vec3 Direction;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;

    // Attenuation
    float Constant;
    float Linear;
    float Quadratic;

    // Spotlight
    float CuttOffAngle;
};

layout(std140, binding = 6) uniform LightArray {
    Light uLights[MAX_LIGHTS];
    uint uCount;
};

vec4 CalculateDirectionalLight(Light light, vec3 normal, vec3 viewDirection, bool materialActive);
vec4 CalculatePointLight(Light light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection, bool materialActive);
vec4 CalculateSpotLight(Light light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection, bool materialActive);

void main() {
    // Properties
    vec4 color = vec4(0.0f);
    const float epsilon = 1e-6;
    vec3 normal = normalize(vNormal);
    vec3 viewDirection = normalize(uViewPosition  - vFragPos);

    // Phong Lighting
    bool materialActive = all(equal(uMaterialAmbientColor, vec3(0.0f))) && all(equal(uMaterialDiffuseColor, vec3(0.0f))) && all(equal(uMaterialSpecularColor, vec3(0.0f)));
    materialActive = false;
    uint count = uCount;
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

    
    oFragColor = vec4(color.rgb, 1.0);
}


vec4 CalculateDirectionalLight(Light light, vec3 normal, vec3 viewDirection, bool materialActive) {
    // Properties
    const float PI = 3.14159265;
    vec3 color = vec3(0.0f);
    vec3 lightDirection = normalize(-light.Direction);
    // Diffuse Shading
    float diffuse = max(dot(normal, lightDirection), 0.0f);
    // Specular Shading
    const float energyConservation = ( 8.0 + uShininess ) / ( 8.0 * PI ); 
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    float specular = energyConservation * pow(max(dot(normal, halfwayDirection), 0.0f), uShininess);
    
    // Ambient lighting
    if (materialActive) {
        color += light.Ambient * texture(uTextureDiffuse, vTexCoords).rgb;
    } else {
        color += light.Ambient * uMaterialAmbientColor;
    }
    
    // Diffuse lighting
    if (materialActive) {
        color += light.Diffuse * diffuse * texture(uTextureDiffuse, vTexCoords).rgb;
    } else {
        color += light.Diffuse * (diffuse * uMaterialDiffuseColor);
    }
    
    // Specular lighting
    if (materialActive) {
        color += light.Specular * specular * texture(uTextureSpecular, vTexCoords).rgb;
    } else {
        color += light.Specular * (specular * uMaterialSpecularColor);
    }
    
    return vec4(color, 1.0f);
}

vec4 CalculatePointLight(Light light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection, bool materialActive) {    
    // Properties
    const float PI = 3.14159265;
    vec3 color = vec3(0.0f);
    vec3 lightDirection = normalize(light.Position - vFragPos);
    // Diffuse Shading
    float diffuse = max(dot(normal, lightDirection), 0.0f);
    // Specular Shading
    const float energyConservation = ( 8.0 + uShininess ) / ( 8.0 * PI ); 
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    float specular = energyConservation * pow(max(dot(normal, halfwayDirection), 0.0f), uShininess);
    // Attenuation
    float distance = length(light.Position - vFragPos);
    float attenuation = 1.0f / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));
    
    // Ambient lighting
    if (materialActive) {
        color += light.Ambient * texture(uTextureDiffuse, vTexCoords).rgb;
    } else {
        color += (light.Ambient * light.Color) * uMaterialAmbientColor;
    }
    color *= attenuation;
    
    // Diffuse lighting
    if (materialActive) {
        color += light.Diffuse * diffuse * texture(uTextureDiffuse, vTexCoords).rgb;
    } else {
        color += (light.Diffuse * light.Color) * (diffuse * uMaterialDiffuseColor);
    }
    color *= attenuation;

    // Specular lighting
    if (materialActive) {
        color += light.Specular * specular * texture(uTextureSpecular, vTexCoords).rgb;
    } else {
        color += (light.Specular * light.Color) * (specular * uMaterialSpecularColor);
    }
    color *= attenuation;

    return vec4(color, 1.0f);
}

vec4 CalculateSpotLight(Light light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection, bool materialActive) {
    // Properties
    const float PI = 3.14159265;
    vec3 color = vec3(0.0f);
    vec3 lightDirection = normalize(light.Position - vFragPos);
    // Diffuse Shading
    float diffuse = max(dot(normal, lightDirection), 0.0f);
    // Specular Shading
    const float energyConservation = ( 8.0 + uShininess ) / ( 8.0 * PI ); 
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    float specular = energyConservation * pow(max(dot(normal, halfwayDirection), 0.0f), uShininess);
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
        color += (light.Ambient * light.Color) * uMaterialAmbientColor;
    }
    color *= attenuation * intensity;
    
    // Diffuse lighting
    if (materialActive) {
        color += light.Diffuse * diffuse * texture(uTextureDiffuse, vTexCoords).rgb;
    } else {
        color += (light.Diffuse * light.Color) * (diffuse * uMaterialDiffuseColor);
    }
    color *= attenuation * intensity;

    // Specular lighting
    if (materialActive) {
        color += light.Specular * specular * texture(uTextureSpecular, vTexCoords).rgb;
    } else {
        color += (light.Specular * light.Color) * (specular * uMaterialSpecularColor);
    }
    color *= attenuation * intensity;

    return vec4(color, 1.0f);
}
