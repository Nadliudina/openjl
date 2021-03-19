#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include "Shader.h"
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

	const int verts = 4;

	float polygon[verts*(3+3)] = {
		0.0f,0.75f,0.0f,	0.0f,0.75f,0.0f,
		0.5f,0.0f,0.0f,		0.5f,0.0f,0.0f,
		-0.5f,0.0f,0.0f,	-0.5f,0.0f,0.0f,
		0.0f,-0.75f,0.0f,	1.0f,-0.75f,0.0f,
	
		
	
	};

	unsigned int indices[] = {
		0,1,2,
		1,2,3
	};

	unsigned int VBO_polygon, VAO_polygon,EBO_polygon;
	glGenBuffers(1, &VBO_polygon);
	glGenBuffers(1, &EBO_polygon);
	glGenVertexArrays(1, &VAO_polygon);

	glBindVertexArray(VAO_polygon);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_polygon);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts * 6, polygon, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_polygon);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6* sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	
	Shader* polygon_shader = new Shader("shaders\\basic.vert","shaders\\basic.frag");

	float color[3] = { 0.0f, 0.0f, 1.0f };
	float position[3] = { 0.0f, 0.0f, 0.0f };
	int stage = 0;
	while (!glfwWindowShouldClose(win))
	{
		color[0] = (sin(glfwGetTime())+1)/2;
		color[1] = (cos(glfwGetTime() + 3.1415) + 1) / 2;
		color[2] =  (-cos(glfwGetTime()) + 1) / 2;

		position[0] = sin(glfwGetTime()*2)/3 ;
		position[1] = cos(glfwGetTime())/3 ;
		position[2] = 0;//(cos(glfwGetTime() + 3.1415) + 1) / 2;
		processInput(win);
		glClearColor(backg.r, backg.g, backg.b, backg.a);
		glClear(GL_COLOR_BUFFER_BIT);
		
		polygon_shader->use();
		polygon_shader->setFloatVec("uniformColor", color, 3);
		polygon_shader->setFloatVec("uniformPos", position, 3);
		glBindVertexArray(VAO_polygon);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(win);
		glfwPollEvents();

	}
	delete polygon_shader;
	glfwTerminate();
	return 0;
}