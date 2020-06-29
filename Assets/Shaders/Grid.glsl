#type vertex
#version 330 core
		
layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TextCoord;

uniform mat4 u_ViewProjection;

out vec2 v_TextCoord;

void main() {
	gl_Position = u_ViewProjection * vec4(Position, 1.0);
	
	v_TextCoord = TextCoord;
}

#type fragment
#version 330 core
		
layout(location = 0) out vec4 color;

uniform float u_Resolution;
uniform float u_Scaling;

in vec2 v_TextCoord;

uniform sampler2D u_Texture;

float calcGrid(vec2 part, float resolution) {
	vec2 grid = fract(part);
	return step(resolution, grid.x) * step(resolution, grid.y);
}

void main() {
	float grid = calcGrid(v_TextCoord * u_Scaling, u_Resolution);
	color = vec4(vec3(0.2), 0.5) * (1.0 - grid);
}
