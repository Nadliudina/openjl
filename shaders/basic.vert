#version 330 core
layout (location = 0 ) in vec3 aPos;
layout (location = 1 ) in vec3 inColor;

out vec3 vertColor;

uniform mat4 model;
void main()
{

	gl_Position = model*vec4(aPos/2, 1.0f);
	vertColor=inColor;
}