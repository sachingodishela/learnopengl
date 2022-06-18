#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;
out vec4 vertexColor;
uniform float xOffset;
void main()
{
	gl_Position = vec4(aPos.x + xOffset, aPos.y, aPos.z, 1.0f);
	vertexColor = vec4(color.x, color.y, color.z, 1.0f);
}