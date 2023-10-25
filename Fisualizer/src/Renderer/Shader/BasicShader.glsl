#shader fragment
#version 330 core

in vec4 fragColor;
out vec4 finalColor;
uniform vec4 color;
void main()
{
	finalColor = fragColor;
}

#shader vertex
#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec4 inColor;
out vec4 fragColor;

uniform mat4 Matrix;
void main()
{
	gl_Position = Matrix * vec4(inPosition, 1.0);
	fragColor = inColor;
}