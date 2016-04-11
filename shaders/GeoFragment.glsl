#version 410 core

layout (location = 0) out vec4 fragColor;

in vec3 fragNormal;
//in vec3 vPosition;

void main()
{
  fragColor = vec4(fragNormal, 1.0);
}
