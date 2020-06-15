#version 330 core
		
layout(location = 0) out vec4 Color;

in vec3 x_Position;
in vec4 x_Color;

void main() {
	Color = x_Color;
}
