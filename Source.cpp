#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h "

#include<iostream>
#include<fstream>
#include "Shader.h"
#include "Camera.h"


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

Camera camera(vec3(0.f, 0.f, -5.f));

bool wireframeMode = false;

void OnResize(GLFWwindow* win, int w, int h) {
	glViewport(0, 0, w, h);
//	cout << "\nonres " << w << " " << h;
}

void processInput(GLFWwindow* win,double dt) {
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
	uint32_t dir = 0;

	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
	{
		dir |= CAM_FORWARD;
	}
	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
	{
		dir |= CAM_BACKWARD;
	}
	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
	{
		dir |= CAM_LEFT;
	}
	if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
	{
		dir |= CAM_RIGHT;
	}

	double newx = 0.f, newy = 0.f;
	glfwGetCursorPos(win, &newx, &newy);
	static double x = newx, y = newy;
	double xoffset = newx - x;
	double yoffset = newy - y;
	x = newx;
	y = newy;

	camera.Move(dir, dt);
	camera.Rotate(xoffset, -yoffset);
}

void OnScroll(GLFWwindow* win, double x, double y)
{
	camera.ChangeFOV(y / 3);
}
void UpdatePolygoneMode()
{
	if (wireframeMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void OnKeyAction(GLFWwindow* win, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_SPACE:
			wireframeMode = !wireframeMode;
			UpdatePolygoneMode();
			break;
		}
	}
}

typedef unsigned char byte;



int main() {
#pragma region WIN_INIT


	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* win = glfwCreateWindow(1600, 900, "OpenGL Window",NULL,NULL);
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
	glfwSetScrollCallback(win, OnScroll);
	glfwSetKeyCallback(win, OnKeyAction);

	glViewport(0, 0, 1600, 900);
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(win, GLFW_CURSOR,GLFW_CURSOR_DISABLED );
	UpdatePolygoneMode();
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

#pragma endregion

	int box_width, box_height, channels;
	byte* data = stbi_load("images\\box.png",&box_width,&box_height,&channels,0);
	const int verts = 8;

	float cube[verts*(3+3+2)] = {
	-1.0f, 1.0f,-1.0f,	    1.0f,0.0f,0.0f, 0.f,1.f,
	 1.0f, 1.0f,-1.0f,	    0.5f,0.5f,0.0f, 1.f,1.f,
	 1.0f, 1.0f, 1.0f,		0.0f,1.0f,0.0f, 1.f,0.f,
	-1.0f, 1.0f, 1.0f,	    0.0f,0.5f,0.5f, 0.f,0.f,
	
	-1.0f,-1.0f,-1.0f,	    0.0f,0.0f,0.1f, 1.f,0.f,
	 1.0f,-1.0f,-1.0f,	    0.5f,0.0f,0.5f, 0.f,0.f,
	 1.0f,-1.0f, 1.0f,	    0.5f,0.5f,0.5f, 0.f,1.f,
	-1.0f,-1.0f, 1.0f,	    1.0f,1.0f,1.0f, 1.f,1.f,

	};

	unsigned int indices[] = {
		0,1,3,
		1,2,3,
		0,4,1,
		1,4,5,
		0,3,7,
		0,7,4,
		1,6,2,
		1,5,6,
		2,7,3,
		2,6,7,
		4,7,5,
		5,7,6,
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
	unsigned int box_texture;
	glGenTextures(1, &box_texture);
	glBindTexture(GL_TEXTURE_2D, box_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	if (channels==3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, box_width, box_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, box_width, box_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	//glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	unsigned int VBO_polygon, VAO_polygon,EBO_polygon;
	glGenBuffers(1, &VBO_polygon);
	glGenBuffers(1, &EBO_polygon);
	glGenVertexArrays(1, &VAO_polygon);

	glBindVertexArray(VAO_polygon);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_polygon);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts *8, cube, GL_STATIC_DRAW);///
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_polygon);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 36, indices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
#pragma endregion

	Shader* polygon_shader = new Shader("shaders\\basic.vert","shaders\\basic.frag");
	
	
	double oldTime = glfwGetTime(), newTime,deltaTime ;
	while (!glfwWindowShouldClose(win))
	{
		newTime = glfwGetTime();
		deltaTime = newTime-oldTime;
		oldTime = newTime;
		processInput(win, deltaTime*100.f);
	//	camera.Rotate(glfwGetTime() * 0.1f, 0, 0);

	//	polygonTrans1.rotation.x = glfwGetTime() * 30;
		polygonTrans1.rotation.z = glfwGetTime() * 90;
		polygonTrans1.position.x = (sin(glfwGetTime()) * 1.9f);
		polygonTrans1.position.y = (cos(glfwGetTime()) * 0.7f);
		polygonTrans1.setScale(0.2f);

		polygonTrans2.rotation.x = glfwGetTime() * 60;
	//	polygonTrans2.rotation.y = glfwGetTime() * 60;
	//	polygonTrans2.position.x = -(sin(glfwGetTime()) * 0.9f);
	//	polygonTrans2.position.y = -(cos(glfwGetTime()) * 0.7f);
		polygonTrans2.setScale(0.4f);

		glClearColor(backg.r, backg.g, backg.b, backg.a);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
		
		polygon_shader->use();

		mat4 pv = camera.GetProjectionMatrix() * camera.GetViewMatrix();
		mat4 pvm ;

		mat4 model = mat4(1.0f);
		model = translate(model, polygonTrans1.position);
		model = rotate(model, radians(polygonTrans1.rotation.x), vec3(1.0f, 0.0f, 0.0f));
		model = rotate(model, radians(polygonTrans1.rotation.y), vec3(0.0f, 1.0f, 0.0f));
		model = rotate(model, radians(polygonTrans1.rotation.z), vec3(0.0f, 0.0f, 1.0f));
		model = scale(model, polygonTrans1.scale);

		pvm = pv * model;
		polygon_shader->setMatrix4F("pvm", pvm);
		polygon_shader->setBool("wireframeMode", wireframeMode);

		glBindTexture(GL_TEXTURE_2D, box_texture);
	//	glBindVertexArray(VAO_polygon);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		
		/////////////////////

		model=mat4(1.0f);
		model = translate(model, polygonTrans2.position);
		model = rotate(model, radians(polygonTrans2.rotation.x), vec3(1.0f, 0.0f, 0.0f));
		model = rotate(model, radians(polygonTrans2.rotation.y), vec3(0.0f, 1.0f, 0.0f));
		model = rotate(model, radians(polygonTrans2.rotation.z), vec3(0.0f, 0.0f, 1.0f));
		model = scale(model, polygonTrans2.scale);
		
		pvm = pv * model;
		polygon_shader->setMatrix4F("pvm", pvm);
		polygon_shader->setBool("wireframeMode", wireframeMode);
	//	glBindVertexArray(VAO_polygon);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(win);
		glfwPollEvents();

	}
	delete polygon_shader;
	glfwTerminate();
	return 0;
}