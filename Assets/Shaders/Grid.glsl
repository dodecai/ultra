#type vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTextureCoordinates;

out vec2 vTextureCoordinates;

uniform mat4 uViewProjection;
uniform mat4 uTransform;

void main() {
	gl_Position = uViewProjection * uTransform * vec4(aPosition, 1.0);
	
	vTextureCoordinates = aTextureCoordinates;
}

#type fragment
#version 330 core
		
layout(location = 0) out vec4 fColor;

in vec2 vTextureCoordinates;

uniform float uResolution;
uniform float uScaling;

float calcGrid(vec2 position, float resolution) {
	vec2 grid = fract(position);
	return step(resolution, grid.x) * step(resolution, grid.y);
}

void main() {
	float xPos = calcGrid(vTextureCoordinates * uScaling, uResolution);
	fColor = vec4(vec3(0.2), 0.5) * (1.0 - xPos);
}
