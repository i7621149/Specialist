#version 410 core

layout (location = 0) out vec4 fragColor;

in vec3 fragNormal;
//in vec3 vPosition;

uniform float alpha;
uniform vec4 color;
uniform bool simpleGhosting;

void main()
{
  float finalAlpha = alpha;
  vec3 finalColor;
  if(fragNormal.x < 0){
    finalAlpha *= -1;
  }

  if(color.a <=0 && simpleGhosting == true){
    finalColor = color.xyz;
  }
  else{
    finalColor = color.xyz * (fragNormal.x);
  }
  fragColor = vec4(finalColor, finalAlpha);
}
