#version 330 core
		
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

uniform mat4 u_ViewProjection;

out vec3 x_Position;
out vec4 x_Color;
		
void main() {
	x_Position = a_Position;
	x_Color = a_Color;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}
