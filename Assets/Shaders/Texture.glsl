// Basic Texture Shader
#type vertex
#version 450 core
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : enable

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in float aTexIndex;
layout(location = 4) in float aTilingFactor;

uniform mat4 uViewProjection;

layout(location = 0) out vec4 oColor;
layout(location = 1) out vec2 oTexCoord;
layout(location = 2) out float oTexIndex;
layout(location = 3) out float oTilingFactor;

void main() {
	oColor = aColor;
	oTexCoord = aTexCoord;
	oTexIndex = aTexIndex;
	oTilingFactor = aTilingFactor;

	gl_Position = uViewProjection * vec4(aPosition, 1.0);
}

#type fragment
#version 450 core
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : enable

layout(location = 0) out vec4 color;

layout(location = 0) in vec4 aColor;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in float aTexIndex;
layout(location = 3) in float aTilingFactor;

layout(binding=0) uniform sampler2D uTextures[32];

void main() {
    color = texture(uTextures[int(aTexIndex)], aTexCoord * aTilingFactor) * aColor;
}
