#pragma once

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
using namespace std;

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();
	void use();
	void setBool(const string& name, bool value) const;
	void setInt(const string& name, int value)const;
	void setFloat(const string& name, float value)const;
	void setFloatVec(const string& name, float* vec, int vec_size) const;
	unsigned int ID();

private:
	unsigned int programID;
	void checkCompileErrors(unsigned int shader, string type);
};

