#version 410 core

#define PI 3.14159265359

#define CIRCLE 0
#define BAR 1

#define NONE 0
#define SPIN_CCW 1
#define SPIN_CW 2
#define ROTATE_1_R 3
#define ROTATE_1_L 4
#define ROTATE_2_R 5
#define ROTATE_2_L 6
#define ROTATE_3_R 7
#define ROTATE_3_L 8
#define KEY 9
#define FINISH 10
#define COLOR_L 11
#define COLOR_R 12
#define COLOR_SET 13

out vec4 outColor;

in vec4 fragColor;
in vec2 fragPos;
in vec2 fragSize;
in float fragSelectedTime;
in float fragClickedTime;
in float fragAction;

in float fragCurrentTime;

uniform sampler2D tex;

uniform vec2 resolution;
uniform vec4 clickedColor;
uniform float circleSize;
uniform float dwellTime;
uniform int borderSize;
uniform int loadingBarSize;
uniform vec4 borderColor;
uniform vec4 loadingBarColor;
uniform int mode;
uniform int animLength;

uniform float hueChangeAmount;

uniform vec4 originalColor;

float MIN(float a, float b, float c)
{
  if(a < b && a < c) return a;
  if(b < c) return b;
  return c;
}

float MAX(float a, float b, float c)
{
  if(a > b && a > c) return a;
  if(b > c) return b;
  return c;
}

vec4 RGBtoHSV( float r, float g, float b )
{
  float min, max, delta;
  float h, s, v;

  min = MIN( r, g, b );
  max = MAX( r, g, b );
  v = max;				// v

  delta = max - min;

  if( max != 0 )
    s = delta / max;		// s
  else {
    // r = g = b = 0		// s = 0, v is undefined
    s = 0;
    h = 0;
    return vec4(h, s, v, 1.0);
  }

  if( r == max )
    h = ( g - b ) / delta;		// between yellow & magenta
  else if( g == max )
    h = 2 + ( b - r ) / delta;	// between cyan & yellow
  else
    h = 4 + ( r - g ) / delta;	// between magenta & cyan

  h *= 60;				// degrees
  if( h < 0 )
    h += 360;
  return vec4(h, s, v, 1.0);
}

vec4 HSVtoRGB( float h, float s, float v)
{
  int i;
  float f, p, q, t;
  float r, g, b;

  if( s == 0 ) {
    // achromatic (grey)
    r = g = b = v;
    return vec4(r, g, b, 1.0);
  }

  h /= 60;			// sector 0 to 5
  i = int( h );
  f = h - i;			// factorial part of h
  p = v * ( 1 - s );
  q = v * ( 1 - s * f );
  t = v * ( 1 - s * ( 1 - f ) );

  switch( i ) {
    case 0:
      r = v;
      g = t;
      b = p;
      break;
    case 1:
      r = q;
      g = v;
      b = p;
      break;
    case 2:
      r = p;
      g = v;
      b = t;
      break;
    case 3:
      r = p;
      g = q;
      b = v;
      break;
    case 4:
      r = t;
      g = p;
      b = v;
      break;
    default:		// case 5:
      r = v;
      g = p;
      b = q;
      break;
  }
  return vec4(r, g, b, 1.0);
}

void main()
{
  float timeSinceClick = fragCurrentTime - fragClickedTime;

  vec2 newPos = (fragPos + 1.0) / 2.0;

  vec2 newSize = fragSize / 2.0;

  vec2 posCoord = newPos * resolution;
  vec2 sizeCoord = newSize * resolution;
  vec2 posCoord2 = posCoord + sizeCoord;

  vec2 buttonUV = (gl_FragCoord.xy - posCoord) / sizeCoord;

  vec2 centerUV = ( fragPos + (fragSize/2.0) + 1.0 ) / 2.0;
  vec2 centerCoord = (newPos + fragSize/4.0) * resolution;

  vec2 line = gl_FragCoord.xy - centerCoord;
  float dist = distance(gl_FragCoord.xy, centerCoord);

  outColor = fragColor;

  vec4 buttonColor = outColor;


  if(mode == CIRCLE){
    //outColor = baseColor;
  }
  if(mode == CIRCLE && fragSelectedTime > 0){
    if(dist > circleSize){
      outColor = buttonColor;
    }
    else if(dist < circleSize - borderSize){
      if(timeSinceClick > dwellTime/5.0){
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
          //outColor = vec4(0.2, 0.6, 1.0, 1.0);
          outColor = loadingBarColor;



          //outColor = fragColor;




        }
      }
      else{
        outColor = clickedColor;

      }
    }
    else{
      outColor = borderColor;



    }
  }
  else{
    if(mode == BAR){
      if(gl_FragCoord.y < posCoord.y + borderSize + loadingBarSize){
        if(buttonUV.x < fragSelectedTime / dwellTime){
          outColor = loadingBarColor;
        }
      }
    }
    else if(fragSelectedTime > 0){
      outColor = fragColor;
    }

    if(mode == CIRCLE){
      outColor = buttonColor;
    }
    else if(timeSinceClick < dwellTime/5.0){
      outColor = clickedColor;
    }

  }

  if(gl_FragCoord.x < posCoord.x + borderSize ||
     gl_FragCoord.x >= posCoord2.x - borderSize ||
     gl_FragCoord.y < posCoord.y + borderSize ||
     gl_FragCoord.y >= posCoord2.y - borderSize){
    outColor = borderColor;
  }
}


