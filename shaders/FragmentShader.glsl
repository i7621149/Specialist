#version 410 core

#define PI 3.14159265359

out vec4 outColor;

in vec4 fragColor;
in vec2 fragPos;
in vec2 fragSize;
in float fragSelectedTime;

uniform sampler2D tex;

uniform vec2 resolution;
uniform float circleSize;
uniform float dwellTime;

void main()
{
  vec2 newPos = fragPos;
  newPos += 1.0;
  newPos /= 2.0;

  vec2 uv = (gl_FragCoord.xy / resolution);

  vec2 centerUV = ( fragPos + (fragSize/2.0) + 1.0 ) / 2.0;
  vec2 centerCoord = (newPos + fragSize/4.0) * resolution;


  vec2 line = gl_FragCoord.xy - centerCoord;
  float dist = distance(gl_FragCoord.xy, centerCoord);

  float angle;

  if(dist > circleSize || angleT > fragSelectedTime){
    outColor = vec4(1.0, 0.0, 0.0, 1.0);
  }
  else{
    if(fragSelectedTime > dwellTime){
      outColor = vec4(0.0, 1.0, 0.0, 1.0);
    }
    else{
      outColor = vec4(1.0, 1.0, 0.0, 1.0);
    }
  }

}
