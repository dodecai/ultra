#type vertex

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in float aTexIndex;
layout(location = 4) in float aTilingFactor;

layout(location = 0) out vec4 vColor;
layout(location = 1) out vec2 vTexCoord;
layout(location = 2) out float vTexIndex;
layout(location = 3) out float vTilingFactor;

uniform mat4 uProjection;
uniform mat4 uView;

void main() {
    vColor = aColor;
    vTexCoord = aTexCoord * aTilingFactor;
    vTexIndex = aTexIndex;
    vTilingFactor = aTilingFactor;

    gl_Position = uProjection * uView * vec4(aPosition, 1.0);
}

#type fragment

layout(location = 0) in vec4 vColor;
layout(location = 1) in vec2 vTexCoord;
layout(location = 2) in float vTexIndex;
layout(location = 3) in float vTilingFactor;

layout(location = 0) out vec4 oColor;

layout(binding = 0) uniform sampler2D uTextures[32];

void main() {
    oColor = texture(uTextures[int(vTexIndex)], vTexCoord * vTilingFactor) * vColor;
}
