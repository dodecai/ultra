// Basic Circle Shader
#type vertex
#version 450 core
#include <Buffers.glslh>

layout(location = 0) in vec3 aWorldPosition;
layout(location = 1) in vec3 aLocalPosition;
layout(location = 2) in vec4 aColor;
layout(location = 3) in float aThickness;
layout(location = 4) in float aFade;

struct VertexOutput {
	vec3 LocalPosition;
	vec4 Color;
	float Thickness;
	float Fade;
};

layout (location = 0) out VertexOutput Output;

void main() {
	Output.LocalPosition = aLocalPosition;
	Output.Color = aColor;
	Output.Thickness = aThickness;
	Output.Fade = aFade;

	gl_Position = uCamera.ViewProjection * vec4(aWorldPosition, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 oColor;

struct VertexOutput {
	vec3 LocalPosition;
	vec4 Color;
	float Thickness;
	float Fade;
};

layout (location = 0) in VertexOutput Input;

void main() {
    // Calculate distance and fill circle with white
    float distance = 1.0 - length(Input.LocalPosition);
    float circle = smoothstep(0.0, Input.Fade, distance);
    circle *= smoothstep(Input.Thickness + Input.Fade, Input.Thickness, distance);

	if (circle == 0.0) discard;

    // Set output color
    oColor = Input.Color;
	oColor.a *= circle;
}