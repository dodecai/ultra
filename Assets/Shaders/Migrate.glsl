#type vertex
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}

#type fragment
out vec4 FragColor;

in vec2 TexCoord;

layout (binding = 0) uniform sampler2D texture1;
uniform vec4 color;

void main()
{
    FragColor = texture(texture1, TexCoord) * color;
}