#version 430 core
out vec4 fragColor;
in vec4 vertexColor;
in vec2 texCoord;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	fragColor = mix(texture(texture1, texCoord), texture(texture2, vec2(texCoord.x, -texCoord.y)), 0.5);
}