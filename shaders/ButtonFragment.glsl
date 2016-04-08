#version 410 core

#define PI 3.14159265359

#define DWELLING 0
#define SCANNING 1
#define TOUCHSCREEN 2

out vec4 outColor;

in vec4 fragColor;
in vec2 fragPos;
in vec2 fragSize;
in float fragSelectedTime;
in float fragClickedTime;

in float fragCurrentTime;

uniform sampler2D tex;

uniform vec2 resolution;
uniform float circleSize;
uniform float dwellTime;
uniform int borderSize;
uniform vec4 borderColor;
uniform int mode;

void main()
{
  float timeSinceClick = fragCurrentTime - fragClickedTime;

  vec2 newPos = (fragPos + 1.0) / 2.0;

  vec2 newSize = fragSize / 2.0;

  vec2 posCoord = newPos * resolution;
  vec2 sizeCoord = newSize * resolution;
  vec2 posCoord2 = posCoord + sizeCoord;

  vec2 uv = (gl_FragCoord.xy / resolution);

  vec2 centerUV = ( fragPos + (fragSize/2.0) + 1.0 ) / 2.0;
  vec2 centerCoord = (newPos + fragSize/4.0) * resolution;

  vec2 line = gl_FragCoord.xy - centerCoord;
  float dist = distance(gl_FragCoord.xy, centerCoord);

  outColor = fragColor;

  if(mode == DWELLING){
    if(dist > circleSize ){
      outColor = fragColor;
    }
    else if(dist < circleSize - borderSize){
      if(timeSinceClick > 0.5){
        float angle = 1.0-(fragSelectedTime / dwellTime);

        float fragAngle = atan(line.y/line.x);
        if(line.x > 0){
          fragAngle += PI * 3.0/2.0;
          //outColor = vec4(fragAngle, 0.0, 0.0, 1.0);
        }
        else{
          fragAngle += PI / 2.0;
          outColor = vec4(0.0, 0.0, 1.0, 1.0);
        }
        fragAngle /= PI * 2.0;

        outColor = vec4(angle, 0.0, 0.0, 1.0);


        if(fragAngle<angle){
          outColor = fragColor;
        }
        else{
          outColor = vec4(0.2, 0.6, 1.0, 1.0);
        }
        //outColor = vec4(0.0, 0.0, fragAngle/(2 * PI), 1.0);
        //outColor = vec4(0.2, 0.4, 0.8, 1.0);
      }
      else{
        outColor = vec4(0.3, 1.0, 0.6, 1.0);

      }
    }
    else{
      outColor = borderColor;
    }
  }
  else if(mode == SCANNING){
    if(timeSinceClick < 0.5){
      outColor = vec4(0.3, 1.0, 0.6, 1.0);
    }
  }

  if(gl_FragCoord.x < posCoord.x + borderSize ||
     gl_FragCoord.x >= posCoord2.x - borderSize ||
     gl_FragCoord.y < posCoord.y + borderSize ||
     gl_FragCoord.y >= posCoord2.y - borderSize){
    outColor = borderColor;
  }

  //outColor = vec4(timeSinceClick, 0.0, 0.0, 1.0);
}
