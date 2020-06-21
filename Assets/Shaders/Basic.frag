#version 330 core
		
layout(location = 0) out vec4 Color;

in vec3 x_Position;
in vec4 x_Color;

uniform vec3 u_Color;

void main() {
	Color = vec4(u_Color, 1.0);
}
