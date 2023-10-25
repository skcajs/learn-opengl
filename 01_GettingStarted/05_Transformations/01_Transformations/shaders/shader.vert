# version 330 core

layout (location=0) in vec3 aPos;
layout (location=2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 uTransform;

void main()
{
	gl_Position = uTransform * vec4(aPos, 1.0f);
	TexCoord = aTexCoord;
};