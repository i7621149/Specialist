#version 410 core
layout (location = 0) in vec4 inPosAndSize;

layout (location = 1) in vec4 inColor;

layout (location = 2) in vec2 inSelectedTimeAndClicked;

out vec4 buttonColor;
out vec2 buttonPos;
out vec2 buttonSize;
out float buttonSelectedTime;
out float buttonClickedTime;

void main()
{
  gl_PointSize = 100.0;

  buttonColor = inColor;

  buttonPos = inPosAndSize.xy;
  buttonSize = inPosAndSize.zw;

  buttonSelectedTime = inSelectedTimeAndClicked[0];
  buttonClickedTime = inSelectedTimeAndClicked[1];

  gl_Position = vec4(inPosAndSize.x, inPosAndSize.y, -1.0, 1.0);
}
