#version 330 core
 
in vec2 theUV;
in vec3 theColor;
 
out vec4 color;
 
uniform sampler2D sourceTexture;
uniform vec2 BarrelPower;
const float PI = 3.1415926535;
uniform vec2 lensCenter;
uniform vec4 distortionParams = vec4(1, 0.22, 0.24, 0);

float getDistortionScale(vec2 offset)
{
  vec2 offsetSquared = offset * offset;
  float rSquared = offsetSquared.x + offsetSquared.y;
  float distortionScale = distortionParams[0] +
      (distortionParams[1] * rSquared) +
      (distortionParams[2] * rSquared * rSquared) +
      (distortionParams[3] * rSquared * rSquared * rSquared);

  return distortionScale;
}

vec2 distort(vec2 p)
{
    p = p - lensCenter;
    float distortionScale = getDistortionScale(p);
    return (((((p * distortionScale) + lensCenter) + 1.0) * 0.5) * 0.5) + 0.25;

    // float theta  = atan(p.y - lensCenter.y, p.x - lensCenter.x);
    // float radius = length(p - lensCenter);
    // radius = pow(radius, BarrelPower.x);
    // p.x = lensCenter.x + radius * cos(theta);
    // p.y = lensCenter.y + radius * sin(theta);
    // return 0.5 * (p + 1.0);
}
 
void main(){
    color = texture( sourceTexture, distort(vec2((fract(theUV.x) * 2.0) - 1.0, theUV.y)) ).rgba;
}