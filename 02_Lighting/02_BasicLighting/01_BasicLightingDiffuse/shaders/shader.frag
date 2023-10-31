# version 330 core

out vec4 FragColor;

uniform vec3 uObjectColour;
uniform vec3 uLightColour;
uniform vec3 uLightPos;

uniform float uAmbient;

in vec3 FragPos;
in vec3 Normal;

void main()
{
	vec3 ambient = uAmbient * uLightColour;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(uLightPos - FragPos);
	float cosineAngle = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = cosineAngle * uLightColour;

	vec3 result = (ambient + diffuse) * uObjectColour;
	FragColor = vec4(result, 1.0);
};