#include "Shader.h"

unsigned int Shader::ID()
{
	return programID;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	const char* vShaderCode;
	const char* fShaderCode;

	string vTempString;
	string fTempString;

	ifstream vShaderFile;
	ifstream fShaderFile;
	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

	try
	{
		stringstream vShaderStream;
		vShaderFile.open(vertexPath);
		vShaderStream << vShaderFile.rdbuf();
		vShaderFile.close();
		vTempString = vShaderStream.str();
		vShaderCode = vTempString.c_str();

		stringstream fShaderStream;
		fShaderFile.open(fragmentPath);
		fShaderStream << fShaderFile.rdbuf();
		fShaderFile.close();
		fTempString = fShaderStream.str();
		fShaderCode = fTempString.c_str();
	}
	catch (ifstream::failure& e)
	{
		cout << " error::shader::file_not_succesfully_read" << endl;
	}

	unsigned int vertex;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	unsigned int fragment;
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	programID = glCreateProgram();
	glAttachShader(programID, vertex);
	glAttachShader(programID, fragment);
	glLinkProgram(programID);
	checkCompileErrors(programID, "PROGRAM");

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader()
{
	glDeleteProgram(programID);
}

void Shader::use()
{
	glUseProgram(programID);
}

void Shader::setBool(const string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);///1b??
}

void Shader::setInt(const string& name, int value) const
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::setFloat(const string& name, float value) const
{
	glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::setFloatVec(const string& name, float * vec,int vec_size) const
{
	switch (vec_size)
	{
	case 1:	glUniform1f(glGetUniformLocation(programID, name.c_str()), *vec);break;
	case 2:	glUniform2f(glGetUniformLocation(programID, name.c_str()), *vec,*(vec+1));break;
	case 3:	glUniform3f(glGetUniformLocation(programID, name.c_str()), *vec, *(vec + 1), *(vec + 2));break;
	case 4:	glUniform4f(glGetUniformLocation(programID, name.c_str()), *vec, *(vec + 1), *(vec + 2), *(vec + 3));break;

	default:
		cout << "shader failure ! uniform vec float size" << endl; 
		break;
	}
 
}

void Shader::checkCompileErrors(unsigned int shader, string type)
{
	int success;
	char infolog[1024];
	if (type!="PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infolog);
			cout << "error::shader_compilation_error of type : " << type << endl << infolog << endl << "-------------------------";
		}
	}
	else
	{
		glGetShaderiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infolog);
			cout << "error::program_compilation_error of type : " << type << endl << infolog << endl << "-------------------------";
		}
	}
}
