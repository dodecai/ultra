// Grid Shader
#type vertex
#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 aPosition;
layout(location = 0) out vec3 vNearClip;
layout(location = 1) out vec3 vFarClip;

layout(std140, binding = 0) uniform Camera {
    mat4 uViewProjection;
    mat4 uProjection;
    mat4 uView;
    float uNearClip;
    float uFarClip;
};

vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection) {
    mat4 invertedView = inverse(view);
    mat4 invertedProjection = inverse(projection);
    vec4 unprojectedPoint =  invertedView * invertedProjection * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
    vec3 position = aPosition.xyz;

    vNearClip = UnprojectPoint(position.x, position.y, 0.0, uView, uProjection).xyz;
    vFarClip = UnprojectPoint(position.x, position.y, 1.0, uView, uProjection).xyz;

    gl_Position = vec4(position, 1.0);
}

#type fragment
#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 vNearClip;
layout(location = 1) in vec3 vFarClip;
layout(location = 2) in mat4 vFragmentView;
layout(location = 6) in mat4 vFramgentProjection;
layout(location = 0) out vec4 oColor;

layout(std140, binding = 0) uniform Camera {
    mat4 uViewProjection;
    mat4 uView;
    mat4 uProjection;
    float uNearClip;
    float uFarClip;
};

vec4 CalculateGridColors(vec3 fragPos3D, float scale, bool drawAxis) {
    vec2 coord = fragPos3D.xz * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    float minimumz = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);
    vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));

    if(fragPos3D.x > -0.1 * minimumx && fragPos3D.x < 0.1 * minimumx) color.z = 1.0;
    if(fragPos3D.z > -0.1 * minimumz && fragPos3D.z < 0.1 * minimumz) color.x = 1.0;
    return color;
}

float ComputeDepth(vec3 position) {
    vec4 clipSpacePosition = vFramgentProjection * vFragmentView * vec4(position.xyz, 1.0);
    return (clipSpacePosition.z / clipSpacePosition.w);
}

float ComputeLinearDepth(vec3 position) {
    vec4 clipSpacePosition = vFramgentProjection * vFragmentView * vec4(position.xyz, 1.0);
    float clipSpaceDepth = (clipSpacePosition.z / clipSpacePosition.w) * 2.0 - 1.0;

    // Set linear depth between near and far clip and normalize it
    float linearDepth = (2.0 * uNearClip * uFarClip) / (uFarClip + uNearClip - clipSpaceDepth * (uFarClip - uNearClip));
    return linearDepth / uFarClip;
}

void main() {
    float t = -vNearClip.y / (vFarClip.y - vNearClip.y);
    vec3 fragPos3D = vNearClip + t * (vFarClip - vNearClip);
    
    gl_FragDepth = ComputeDepth(fragPos3D);
    float linearDepth = ComputeLinearDepth(fragPos3D);
    float fading = max(0, (0.5 - linearDepth));

    oColor = CalculateGridColors(fragPos3D, 10, true) * float(t > 0);
    //oColor *= fading;
}
