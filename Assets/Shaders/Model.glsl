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

layout(std140, binding = 4) uniform Material {
    vec3 uAmbientColor;
    vec3 uDiffuseColor;
    vec3 uSpecularColor;
    float uShininess;
};

layout(std140, binding = 5) uniform Light {
    vec3 uLightColor;
    vec3 uLightPosition;
    vec3 uLightDirection;
    vec3 uAmbient;
    vec3 uDiffuse;
    vec3 uSpecular;

    // Attenuation
    float uConstant;
    float uLinear;
    float uQuadratic;

    // Spotlight
    float uCuttOffAngle;
};

layout(std140, binding = 6) uniform View {
    vec3 uViewPosition;
};


vec4 CalculateDirectionalLight(vec3 normal, vec3 viewDirection, bool materialActive);
vec4 CalculatePointLight(vec3 normal, vec3 fragmentPosition, vec3 viewDirection, bool materialActive);
vec4 CalculateSpotLight(vec3 normal, vec3 fragmentPosition, vec3 viewDirection, bool materialActive);

void main() {
    // Properties
    vec4 color = vec4(0.0f);
    const float epsilon = 1e-6;
    vec3 normal = normalize(vNormal);
    vec3 viewDirection = normalize(uViewPosition  - vFragPos);

    // Phong Lighting
    bool materialActive = all(equal(uAmbientColor, vec3(0.0f))) && all(equal(uDiffuseColor, vec3(0.0f))) && all(equal(uSpecularColor, vec3(0.0f)));
    materialActive = false;
    // Phase 1: Directional Lighting
    //color += CalculateDirectionalLight(normal, viewDirection, materialActive);
    // Phase 2: Point Lighting
    color += CalculatePointLight(normal, vFragPos, viewDirection, materialActive);
    // Phase 3: Spotlight
    //color += CalculateSpotLight(normal, vFragPos, viewDirection, materialActive);
    
    oFragColor = vec4(color.rgb, 1.0);
}


vec4 CalculateDirectionalLight(vec3 normal, vec3 viewDirection, bool materialActive) {
    // Properties
    vec3 color = vec3(0.0f);
    vec3 lightDirection = normalize(-uLightDirection);
    // Diffuse Shading
    float diffuse = max(dot(normal, lightDirection), 0.0f);
    // Specular Shading
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specular = pow(max(dot(viewDirection, reflectDirection), 0.0f), uShininess);
    
    // Ambient lighting
    if (materialActive) {
        color += uAmbient * texture(uTextureDiffuse, vTexCoords).rgb;
    } else {
        color += uAmbientColor * uAmbient * uDiffuseColor;
    }
    
    // Diffuse lighting
    if (materialActive) {
        color += uDiffuse * diffuse * texture(uTextureDiffuse, vTexCoords).rgb;
    } else {
        color += uDiffuseColor * diffuse * uDiffuseColor;
    }

    // Specular lighting
    if (materialActive) {
        color += uSpecular * specular * texture(uTextureSpecular, vTexCoords).rgb;
    } else {
        color += uSpecularColor * specular * uSpecular * uSpecularColor;
    }
    
    return vec4(color, 1.0f);
}

vec4 CalculatePointLight(vec3 normal, vec3 fragmentPosition, vec3 viewDirection, bool materialActive) {    
    // Properties
    vec3 color = vec3(0.0f);
    vec3 lightDirection = normalize(uLightPosition - vFragPos);
    // Diffuse Shading
    float diffuse = max(dot(normal, lightDirection), 0.0f);
    // Specular Shading
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specular = pow(max(dot(viewDirection, reflectDirection), 0.0f), uShininess);
    // Attenuation
    float distance = length(uLightPosition - vFragPos);
    float attenuation = 1.0f / (uConstant + uLinear * distance + uQuadratic * (distance * distance));
    
    // Ambient lighting
    if (materialActive) {
        color += uAmbient * texture(uTextureDiffuse, vTexCoords).rgb;
    } else {
        color += uAmbientColor * uAmbient * uDiffuseColor;
    }
    color *= attenuation;
    
    // Diffuse lighting
    if (materialActive) {
        color += uDiffuse * diffuse * texture(uTextureDiffuse, vTexCoords).rgb;
    } else {
        color += uDiffuseColor * diffuse * uDiffuseColor;
    }
    color *= attenuation;

    // Specular lighting
    if (materialActive) {
        color += uSpecular * specular * texture(uTextureSpecular, vTexCoords).rgb;
    } else {
        color += uSpecularColor * specular * uSpecular * uSpecularColor;
    }
    color *= attenuation;

    return vec4(color, 1.0f);
}

vec4 CalculateSpotLight(vec3 normal, vec3 fragmentPosition, vec3 viewDirection, bool materialActive) {
    // Properties
    vec3 color = vec3(0.0f);
    vec3 lightDirection = normalize(uLightPosition - vFragPos);
    // Diffuse Shading
    float diffuse = max(dot(normal, lightDirection), 0.0f);
    // Specular Shading
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specular = pow(max(dot(viewDirection, reflectDirection), 0.0f), uShininess);
    // Attenuation
    float distance = length(uLightPosition - vFragPos);
    float attenuation = 1.0f / (uConstant + uLinear * distance + uQuadratic * (distance * distance));
    // Intensity
    float theta = dot(lightDirection, normalize(-uLightDirection));
    float epsilon = uCuttOffAngle - (uCuttOffAngle - 0.05f);
    float intensity = clamp((theta - (uCuttOffAngle - 0.05f)) / epsilon, 0.0f, 1.0f);
        
    // Ambient lighting
    if (materialActive) {
        color += uAmbient * texture(uTextureDiffuse, vTexCoords).rgb;
    } else {
        color += uAmbientColor * uAmbient * uDiffuseColor;
    }
    color *= attenuation * intensity;
    
    // Diffuse lighting
    if (materialActive) {
        color += uDiffuse * diffuse * texture(uTextureDiffuse, vTexCoords).rgb;
    } else {
        color += uDiffuseColor * diffuse * uDiffuseColor;
    }
    color *= attenuation * intensity;

    // Specular lighting
    if (materialActive) {
        color += uSpecular * specular * texture(uTextureSpecular, vTexCoords).rgb;
    } else {
        color += uSpecularColor * specular * uSpecular * uSpecularColor;
    }
    color *= attenuation * intensity;

    return vec4(color, 1.0f);
}
