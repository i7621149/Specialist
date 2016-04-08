#version 410 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in vec4 buttonColor[];
in vec2 buttonPos[];
in vec2 buttonSize[];
in float buttonSelectedTime[];
in float buttonClickedTime[];

uniform float currentTime;
uniform bool clickMovement;

out vec4 fragColor;
out vec2 fragPos;
out vec2 fragSize;
out float fragSelectedTime;
out float fragClickedTime;
out float fragCurrentTime;

void main()
{
  fragColor = buttonColor[0];
  fragSelectedTime = buttonSelectedTime[0];
  fragClickedTime = buttonClickedTime[0];
  fragCurrentTime = currentTime;

  float timeSinceClick = currentTime - buttonClickedTime[0];

  float move = 0.0;

  if(clickMovement){
    float timeOffset = timeSinceClick - 0.01;
    move = clamp(timeOffset * timeOffset, 0.0, 0.01) - 0.01;
  }

  fragPos = buttonPos[0];
  fragSize = buttonSize[0];
  fragPos.x -= move;
  fragPos.y -= move;
  fragSize.x += move*2;
  fragSize.y += move*2;

  gl_Position = gl_in[0].gl_Position + vec4(0.0-move, 0.0-move, 0.0, 0.0);
  EmitVertex();

  gl_Position = gl_in[0].gl_Position + vec4(0.0-move, buttonSize[0].y+move, 0.0, 0.0);
  EmitVertex();

  gl_Position = gl_in[0].gl_Position + vec4(buttonSize[0].x+move, 0.0-move, 0.0, 0.0);
  EmitVertex();

  gl_Position = gl_in[0].gl_Position + vec4(buttonSize[0].x+move, buttonSize[0].y+move, 0.0, 0.0);
  EmitVertex();

  EndPrimitive();
}
