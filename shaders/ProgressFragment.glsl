#version 410 core
out vec4 fragColor;

uniform int frame;
uniform int animLength;
uniform vec4 backgroundColor;
uniform vec2 resolution;
uniform int progressBarSize;
uniform vec4 progressBarColor;
uniform vec4 progressBarSelection;
uniform vec4 progressBarBack;
uniform int borderSize;
uniform vec4 borderColor;
uniform bool colorSet;

void main()
{
  float progress = frame * (1.0 / animLength);
  vec2 uv = gl_FragCoord.xy / resolution;
  fragColor = backgroundColor;

  if(colorSet){
    if(uv.x < progress && gl_FragCoord.y < progressBarSize){
      fragColor = progressBarColor;
    }
    else if(uv.x < progress + (1.0/animLength) && gl_FragCoord.y < progressBarSize){
      fragColor = progressBarSelection;
    }
    else if(gl_FragCoord.y < progressBarSize){
      fragColor = progressBarBack;
    }
    else if(gl_FragCoord.y < progressBarSize + borderSize){
      fragColor = borderColor;
    }
  }
}
