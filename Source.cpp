#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<iostream>
#include<fstream>
#include "Shader.h"
using namespace std;
using namespace glm;

struct ModelTransform
{
	
	vec3 position;
	vec3 rotation;
	vec3 scale ;
	void setScale(float s) {
		scale.x = s;
		scale.y = s;
		scale.z = s;
	}
};

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
#pragma region WIN_INIT
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* win = glfwCreateWindow(900, 900, "OpenGL Window",NULL,NULL);
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

	glViewport(0, 0, 900, 900);
#pragma endregion
	const int verts = 4;

	float polygon[verts*(3+3)] = {
		0.0f,0.75f,0.0f,	1.0f,0.75f,0.0f,
		0.5f,0.0f,0.0f,		0.5f,0.0f,0.0f,
		-0.5f,0.0f,0.0f,	-0.5f,0.0f,0.1f,
		0.0f,-0.75f,0.0f,	1.0f,-0.75f,0.0f,

	};

	unsigned int indices[] = {
		0,1,2,
		1,2,3
	};

	ModelTransform polygonTrans1 = {
		vec3(0.0f,0.0f,0.0f),
		vec3(0.0f,0.0f,0.0f),
		vec3(1.0f,1.0f,1.0f) };

	ModelTransform polygonTrans2 = {
	vec3(0.0f,0.0f,0.0f),
	vec3(0.0f,0.0f,0.0f),
	vec3(1.0f,1.0f,1.0f) };

#pragma region BUFF_IN
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
	
#pragma endregion

	Shader* polygon_shader = new Shader("shaders\\basic.vert","shaders\\basic.frag");
	
//	polygonTrans.position.x = 0.3f;
	
	while (!glfwWindowShouldClose(win))
	{
		processInput(win);

		polygonTrans1.rotation.z = glfwGetTime() * 30;
		polygonTrans1.setScale(abs(sin(glfwGetTime()) * 0.5f) + 0.5f);
		polygonTrans1.position.x = (sin(glfwGetTime()) * 0.9f);
		polygonTrans1.position.y = (cos(glfwGetTime()) * 0.7f);
		polygonTrans1.setScale(0.7f);

		polygonTrans2.rotation.z = glfwGetTime() * 60;
		polygonTrans2.setScale(abs(sin(glfwGetTime()) * 0.5f) + 0.5f);
		polygonTrans2.position.x = -(sin(glfwGetTime()) * 0.9f);
		polygonTrans2.position.y = -(cos(glfwGetTime()) * 0.7f);
		polygonTrans2.setScale(1.7f);

		glClearColor(backg.r, backg.g, backg.b, backg.a);
		glClear(GL_COLOR_BUFFER_BIT);
		
		polygon_shader->use();
		mat4 model = mat4(1.0f);
		model = translate(model, polygonTrans1.position);
		model = rotate(model, radians(polygonTrans1.rotation.x), vec3(1.0f, 0.0f, 0.0f));
		model = rotate(model, radians(polygonTrans1.rotation.y), vec3(0.0f, 1.0f, 0.0f));
		model = rotate(model, radians(polygonTrans1.rotation.z), vec3(0.0f, 0.0f, 1.0f));
		model = scale(model, polygonTrans1.scale);
		polygon_shader->setMatrix4F("model", model);

		glBindVertexArray(VAO_polygon);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		/////////////////////

		model=mat4(1.0f);
		model = translate(model, polygonTrans2.position);
		model = rotate(model, radians(polygonTrans2.rotation.x), vec3(1.0f, 0.0f, 0.0f));
		model = rotate(model, radians(polygonTrans2.rotation.y), vec3(0.0f, 1.0f, 0.0f));
		model = rotate(model, radians(polygonTrans2.rotation.z), vec3(0.0f, 0.0f, 1.0f));
		model = scale(model, polygonTrans2.scale);
		polygon_shader->setMatrix4F("model", model);

	//	glBindVertexArray(VAO_polygon);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(win);
		glfwPollEvents();

	}
	delete polygon_shader;
	glfwTerminate();
	return 0;
}