#type vertex
#version 450 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aSize;
layout(location = 3) in float aInnerAlpha;
layout(location = 4) in float aBevel;
layout(location = 5) in vec2 aCoordinate;

layout(location = 0) out vec3 vPosition;
layout(location = 1) out vec4 vColor;
layout(location = 2) out vec2 vSize;
layout(location = 3) out float vInnerAlpha;
layout(location = 4) out float vBevel;
layout(location = 5) out vec2 vCoordinate;

layout(std140, binding = 0) uniform Transform {
    uniform mat4 uProjection;
    uniform mat4 uModelView;
};

void main() {
  vPosition = aPosition;
  vColor = aColor;
  vSize = aSize;
  vInnerAlpha = aInnerAlpha;
  vBevel = aBevel;
  
  vCoordinate = aCoordinate;
  gl_Position = uProjection * (uModelView * vec4(aPosition, 1.0));
}

#type fragment
#version 450 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec2 vSize;
layout(location = 3) in float vInnerAlpha;
layout(location = 4) in float vBevel;
layout(location = 5) in vec2 vCoordinate;

layout(location = 0) out vec4 oColor;

layout(std140, binding = 1) uniform PanelProperties {
    float uPadding;
};

float dbox(vec2 p, vec2 s, float b) {
  return length(max(vec2(0.0, 0.0), abs(p) - (s - 2.0 * vec2(b, b)))) - b;
}

float saturate(float x) {
  return clamp(x, 0.0, 1.0);
}

void main() {
  vec4 color = vColor;
  vec2 size = vSize;
  float innerAlpha = vInnerAlpha;
  float bevel = vBevel;

  vec3 c;
  c = color.xyz * (1.25 - 0.5 * vCoordinate.y);
  float x = size.x * (2.0 * vCoordinate.x - 1.0);
  float y = size.y * (2.0 * vCoordinate.y - 1.0);

  float d = dbox(vec2(x, y), size + bevel - 2.0 * uPadding, bevel);
  float k = exp(-max(0.0, d));
  float mult = 0.0;

  // Inner opacity
  mult += innerAlpha * k;

  // Shadow
  mult += 0.75 * saturate(exp(-pow(0.2 * max(0.0, d), 0.75)) - k);

  mult *= color.w;
  mult = saturate(mult);

  // Gradient
  c *= 0.8 + 0.4 * exp(-2.0 * vCoordinate.y);

  c += 0.3 * vec3(0.1, 0.5, 1.0) * exp(-8.0 * length(vCoordinate - vec2(0.5, 0.0)));
  c = mix(c, vec3(0.005, 0.005, 0.005), 1.0 - exp(-2.0 * max(0.0, d)));

  oColor = vec4(c, mult);
}
