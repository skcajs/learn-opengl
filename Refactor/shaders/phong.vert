# version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

uniform vec3 uLightPos;

out vec3 LightPos;
out vec3 FragPos;
out vec3 Normal;

void main()
{
	FragPos = vec3(uView * uModel * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(uView * uModel))) * aNormal;
	LightPos = vec3(uView * vec4(uLightPos, 1.0));

	gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
};