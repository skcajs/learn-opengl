#version 330 core

out vec4 FragColor;

uniform vec3 uLightColour;

void main()
{
	FragColor = vec4(uLightColour, 1.0);
}