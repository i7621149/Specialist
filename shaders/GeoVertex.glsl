#version 410 core

//inputs
layout (location = 0) in vec3 inVert;
layout (location = 2) in vec3 inNormal;

//outputs
out vec3 fragNormal;
out vec3 fragOldNormal;
//out vec3 vPosition;

uniform mat4 MVP;
uniform mat3 normalMatrix;

void main()
{
  fragNormal = normalize(normalMatrix * inNormal);
  fragOldNormal = normalize(inNormal);
  gl_Position = MVP * vec4(inVert, 1.0);
}
