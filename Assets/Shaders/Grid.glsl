// Grid Shader
#type vertex
#version 450 core
#include <Buffers.glslh>

layout(location = 0) in vec3 aPosition;

layout(location = 0) out float near;
layout(location = 1) out float far;
layout(location = 2) out vec3 nearPoint;
layout(location = 3) out vec3 farPoint;
layout(location = 4) out mat4 fragView;
layout(location = 8) out mat4 fragProj;

vec3 UnprojectPoint(float x, float y, float z) {
    vec4 unprojectedPoint =  uCamera.InverseView * uCamera.InverseProjection * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

// normal vertice projection
void main() {
    near = uCamera.Near;
    far = uCamera.Far;
    fragView = uCamera.View;
    fragProj = uCamera.Projection;
    vec3 position = aPosition.xyz;

    // Unprojecting the nar/far point
    nearPoint = UnprojectPoint(position.x, position.y, 0.0).xyz;
    farPoint = UnprojectPoint(position.x, position.y, 1.0).xyz;

    // Use directly the clipped coordinates
    gl_Position = vec4(position, 1.0);
}


#type fragment
#version 450 core

layout(location = 0) in float near;
layout(location = 1) in float far;
layout(location = 2) in vec3 nearPoint;
layout(location = 3) in vec3 farPoint;
layout(location = 4) in mat4 fragView;
layout(location = 8) in mat4 fragProj;
layout(location = 0) out vec4 outColor;

float computeDepth(vec3 pos);
float computeLinearDepth(vec3 pos);
vec4 grid(vec3 fragPos3D, float scale, bool drawAxis);

void main() {
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);

    gl_FragDepth = computeDepth(fragPos3D);

    float linearDepth = computeLinearDepth(fragPos3D);
    float fading = max(0, (0.5 - linearDepth));
     
    // adding multiple resolution for the grid
    outColor = (grid(fragPos3D, 10, true) + grid(fragPos3D, 1, true))* float(t > 0);
    outColor.a *= fading;
}

float computeDepth(vec3 pos) {
    vec4 clip_space_pos = fragProj * fragView * vec4(pos.xyz, 1.0);
    return (clip_space_pos.z / clip_space_pos.w);
}

float computeLinearDepth(vec3 pos) {
    vec4 clip_space_pos = fragProj * fragView * vec4(pos.xyz, 1.0);
    float clip_space_depth = (clip_space_pos.z / clip_space_pos.w) * 2.0 - 1.0; // put back between -1 and 1
    float linearDepth = (2.0 * near * far) / (far + near - clip_space_depth * (far - near)); // get linear value between 0.01 and 100
    return linearDepth / far; // normalize
}

vec4 grid(vec3 fragPos3D, float scale, bool drawAxis) {
    vec2 coord = fragPos3D.xz * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    float minimumz = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);
    vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));
    // z axis
    if(fragPos3D.x > -0.1 * minimumx && fragPos3D.x < 0.1 * minimumx) color.z = 1.0;
    // x axis
    if(fragPos3D.z > -0.1 * minimumz && fragPos3D.z < 0.1 * minimumz) color.x = 1.0;
    return color;
}
