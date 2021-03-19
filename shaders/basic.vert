#version 330 core
layout (location = 0 ) in vec3 aPos;
layout (location = 1 ) in vec3 inColor;

out vec3 vertColor;

uniform vec3 uniformPos;
void main()
{
vec3 dir=(uniformPos-vec3(0,0,0))/distance(aPos,uniformPos);

	//gl_Position = vec4(aPos+uniformPos, 1.0f);
	gl_Position = vec4(aPos/2+dir/3.0f+uniformPos/2, 1.0f);
	vertColor=inColor;
}