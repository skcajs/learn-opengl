# version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform vec3 uLightColour;

uniform float uAmbientStrength;
uniform float uSpecularStrength;
uniform int uShininess;

out vec3 Color;

void main()
{
	gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);

	vec3 ambient = uAmbientStrength * uLightColour;

	vec3 vertexPos = vec3(uModel * vec4(aPos, 1.0));
	vec3 Normal = mat3(transpose(inverse(uModel))) * aNormal;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(uLightPos - vertexPos);
	float cosineAngle = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = cosineAngle * uLightColour;

	vec3 viewDir = normalize(uViewPos-vertexPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), uShininess);
	vec3 specular = uSpecularStrength * spec * uLightColour;

	Color = ambient + diffuse + specular;
};