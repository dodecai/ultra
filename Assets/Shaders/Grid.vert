#version 330 core
		
layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TextCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_TextCoord;

void main() {
	v_TextCoord = TextCoord;
	gl_Position = u_ViewProjection * u_Transform * vec4(Position, 1.0);
}
