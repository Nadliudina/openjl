#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<fstream>
using namespace std;
struct Color {
	float r, g, b, a;
};
Color backg = { 0.f, 0.f, 0.f, 1.f };

void OnResize(GLFWwindow* win, int w, int h) {
	glViewport(0, 0, w, h);
//	cout << "\nonres " << w << " " << h;
}

void processInput(GLFWwindow* win) {
	if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(win, true);
	}
	if (glfwGetKey(win, GLFW_KEY_1) == GLFW_PRESS)
	{
		backg = { 1.f, 0.f, 0.f, 1.f };
	}
	if (glfwGetKey(win, GLFW_KEY_2) == GLFW_PRESS)
	{
		backg = { 0.f, 0.f, 1.f, 1.f };
	}
	if (glfwGetKey(win, GLFW_KEY_3) == GLFW_PRESS)
	{
		backg = { 1.f, 1.f, 0.f, 1.f };
	}
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* win = glfwCreateWindow(500, 550, "OpenGL Window",NULL,NULL);
	if (win==NULL)
	{
		cout << "\nerr 1";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(win);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "\nerr 2";
		glfwTerminate();
		return -1;
	}

	glfwSetFramebufferSizeCallback(win, OnResize);

	glViewport(0, 0, 500, 550);

	const int verts = 6;

	float polygon[verts*(4+3)] = {
		-0.5f,0.0f,0.0f,	-0.5f,0.0f,0.0f,1.0f,
		0.0f,0.75f,0.0f,	0.0f,0.75f,0.0f,1.0f,
		0.5f,0.0f,0.0f,		0.5f,0.0f,0.0f,1.0f,

		-0.5f,0.0f,0.0f,   	-0.5f,0.0f,0.0f,1.0f,
		0.0f,-0.75f,0.0f,	1.0f,-0.75f,0.0f,1.0f,
		0.5f,0.0f,0.0f	,	0.5f,0.0f,0.0f,1.0f,
	};

	unsigned int VBO_polygon;
	glGenBuffers(1, &VBO_polygon);

	unsigned VAO_polygon;
	glGenVertexArrays(1, &VAO_polygon);

	glBindVertexArray(VAO_polygon);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_polygon);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts * 7, polygon, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	/*char *shaderText=new char[1000],str[200];
	ifstream sfile;
	sfile.open("basic.vs");
	while (!sfile.eof())
	{
		str[0] = '\0';
		sfile >> str;
		if(str[0]!='\0')
		strcat_s(shaderText,200, str);
	}
	sfile.close();*/

	const char* shaderTextVertex = 
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"layout(location = 1) in vec4 inColor;\n"
		"out vec4 fragColor;\n"
		"void main()\n"
		"{		gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f)\n;"
		"fragColor = inColor;}";

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &shaderTextVertex,NULL);
	glCompileShader(vertexShader);

	char resultInfo[1000];
	int res;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &res);
	if (res==0)
	{
		glGetShaderInfoLog(vertexShader, 1000, NULL, resultInfo);
		cout << "\n err " << resultInfo;
		glfwTerminate();
		return -1;
	}

	/*shaderText[0] = 0;
	sfile.open("basic.fs");
	while (!sfile.eof())
	{
		str[0] = '\0';
		sfile >> str;
		if (str[0] != '\0')
			strcat_s(shaderText, 200, str);
	}
	sfile.close();*/

	const char* shaderTextFragment = "#version 330 core\n"
		"in vec4 fragColor;"
		"out vec4 outColor;\n"
		"void main()\n"
		"{	outColor = fragColor;	 	}";

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &shaderTextFragment, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &res);
	if (res == 0)
	{
		glGetShaderInfoLog(fragmentShader, 1000, NULL, resultInfo);
		cout << "\n err comp " << resultInfo;
		glfwTerminate();
		return -1;
	}

	unsigned int shaderProc;
	shaderProc = glCreateProgram();
	glAttachShader(shaderProc, vertexShader);
	glAttachShader(shaderProc, fragmentShader);
	glLinkProgram(shaderProc);

	glGetProgramiv(shaderProc, GL_LINK_STATUS, &res);
	if (res == 0)
	{
		glGetProgramInfoLog(shaderProc, 1000, NULL, resultInfo);
		cout << "\n err link " << resultInfo;
		glfwTerminate();
		return -1;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	while (!glfwWindowShouldClose(win))
	{
		processInput(win);
		glClearColor(backg.r, backg.g, backg.b, backg.a);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_polygon);
	
		
		glUseProgram(shaderProc);
		glBindVertexArray(VAO_polygon);
		glDrawArrays(GL_TRIANGLES, 0, verts);
		

		glfwSwapBuffers(win);
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}