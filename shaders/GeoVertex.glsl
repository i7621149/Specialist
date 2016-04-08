#version 410 core

//inputs
layout (location = 0) in vec3 inVert;
layout (location = 2) in vec3 inNormal;

//outputs
out vec3 fragmentNormal;
//out vec3 vPosition;

void main()
{
  fragmentNormal = normalize(inNormal);
  gl_Position = vec4(inVert, 1.0);
}
