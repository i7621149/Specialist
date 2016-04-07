#version 410 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in vec4 buttonColor[];
in vec2 buttonPos[];
in vec2 buttonSize[];
in float buttonSelectedTime[];

out vec4 fragColor;
out vec2 fragPos;
out vec2 fragSize;
out float fragSelectedTime;

void main()
{
  fragColor = buttonColor[0];
  fragPos = buttonPos[0];
  fragSize = buttonSize[0];
  fragSelectedTime = buttonSelectedTime[0];

  gl_Position = gl_in[0].gl_Position + vec4(0.0, 0.0, 0.0, 0.0);
  EmitVertex();

  gl_Position = gl_in[0].gl_Position + vec4(0.0, buttonSize[0].y, 0.0, 0.0);
  EmitVertex();

  gl_Position = gl_in[0].gl_Position + vec4(buttonSize[0].x, 0.0, 0.0, 0.0);
  EmitVertex();

  gl_Position = gl_in[0].gl_Position + vec4(buttonSize[0].xy, 0.0, 0.0);
  EmitVertex();

  EndPrimitive();
}
