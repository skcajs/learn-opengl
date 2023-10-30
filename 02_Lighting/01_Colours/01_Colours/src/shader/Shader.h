#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:

	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);

	void use();

	void setBool(const std::string& name, bool value) const;

	void setInt(const std::string& name, int value) const;

	void setFloat(const std::string& name, float value) const;

	void setVec2(const std::string& name, float value0, float value1) const;

	void setVec3(const std::string& name, float value0, float value1, float value2) const;

	void setVec4(const std::string& name, float value0, float value1, float value2, float value3) const;

	void setMat4(const std::string& name, glm::f32*) const;

};

#endif
