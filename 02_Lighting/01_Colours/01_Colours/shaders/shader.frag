# version 330 core

out vec4 FragColor;

uniform vec3 uObjectColour;
uniform vec3 uLightColour;

void main()
{
	FragColor = vec4(uObjectColour * uLightColour, 1.0f);
};