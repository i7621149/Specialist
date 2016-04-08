#version 410 core

layout (location = 0) out vec4 fragColor;

in vec3 fragmentNormal;
//in vec3 vPosition;

void main()
{
  fragColor = vec4(fragmentNormal, 1.0);
}
