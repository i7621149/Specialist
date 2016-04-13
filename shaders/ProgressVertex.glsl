#version 410 core

// positions in
layout (location = 0) in vec3  inPosition;

// simple quad shader, minus parts included in BaseVertex.glsl
void main()
{
  gl_Position = vec4(inPosition, 1.0);
}
