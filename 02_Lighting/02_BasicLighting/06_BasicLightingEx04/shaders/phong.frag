# version 330 core

out vec4 FragColor;

uniform vec3 uObjectColour;
uniform vec3 uLightColour;

uniform float uAmbientStrength;
uniform float uSpecularStrength;
uniform int uShininess;

in vec3 LightPos;
in vec3 FragPos;
in vec3 Normal;

void main()
{
	vec3 ambient = uAmbientStrength * uLightColour;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos);
	float cosineAngle = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = cosineAngle * uLightColour;

	vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), uShininess);
	vec3 specular = uSpecularStrength * spec * uLightColour;

	vec3 result = (ambient + diffuse + specular) * uObjectColour;
	FragColor = vec4(result, 1.0);
};