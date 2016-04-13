#version 410 core
layout (location = 0) in vec4 inPosAndSize;

layout (location = 1) in vec4 inColor;

layout (location = 2) in vec3 inSelectedTimeClickedAction;

layout (location = 3) in float inAction;

out vec4 buttonColor;
out vec2 buttonPos;
out vec2 buttonSize;
out float buttonSelectedTime;
out float buttonClickedTime;
out float buttonAction;

void main()
{
  gl_PointSize = 100.0;

  buttonAction = inSelectedTimeClickedAction[2];

  buttonColor = inColor;

  buttonPos = inPosAndSize.xy;
  buttonSize = inPosAndSize.zw;

  buttonSelectedTime = inSelectedTimeClickedAction[0];
  buttonClickedTime = inSelectedTimeClickedAction[1];

  gl_Position = vec4(inPosAndSize.x, inPosAndSize.y, -1.0, 1.0);
}
