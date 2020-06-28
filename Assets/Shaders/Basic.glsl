#type vertex
#version 330 core
		
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 x_Position;
out vec4 x_Color;
		
void main() {
	x_Position = a_Position;
	x_Color = a_Color;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core
		
layout(location = 0) out vec4 Color;

in vec3 x_Position;
in vec4 x_Color;

uniform vec3 u_Color;

void main() {
	Color = vec4(u_Color, 1.0);
}
