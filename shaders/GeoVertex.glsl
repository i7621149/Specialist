#version 410 core

//inputs
layout (location = 0) in vec3 inVert;
layout (location = 2) in vec3 inNormal;

//outputs
out vec3 fragmentNormal;
//out vec3 vPosition;

uniform mat4 MVP;

void main()
{
  fragmentNormal = normalize(inNormal);
  gl_Position = MVP * vec4(inVert, 1.0);
}
