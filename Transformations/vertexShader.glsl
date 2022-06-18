#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoordIn;

out vec4 vertexColor;
out vec2 texCoord;

uniform float yOffset;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y + yOffset, aPos.z, 1.0f);
	vertexColor = vec4(color.x, color.y, color.z, 1.0f);
	texCoord = texCoordIn;
}