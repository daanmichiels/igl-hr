#ifndef SHADERS_H
#define SHADERS_H

#include <string>
#include <map>

std::map<std::string, std::string> shader_sources = {
{"fragment", "#version 330\n\nsmooth in vec3 theColor;\n\nlayout(location = 0) out vec3 color;\n\nvoid main() {\n    color = (theColor);\n}\n"},
{"rift_fragment", "#version 330 core\n \nin vec2 theUV;\nin vec3 theColor;\n \nout vec4 color;\n \nuniform sampler2D left_texture;\nuniform vec2 BarrelPower;\nconst float PI = 3.1415926535;\nuniform vec2 lensCenter;\nuniform vec4 distortionParams = vec4(1, 0.22, 0.24, 0);\n\nfloat getDistortionScale(vec2 offset)\n{\n  vec2 offsetSquared = offset * offset;\n  float rSquared = offsetSquared.x + offsetSquared.y;\n  float distortionScale = distortionParams[0] +\n      (distortionParams[1] * rSquared) +\n      (distortionParams[2] * rSquared * rSquared) +\n      (distortionParams[3] * rSquared * rSquared * rSquared);\n\n  return distortionScale;\n}\n\nvec2 distort(vec2 p)\n{\n    p = p - lensCenter;\n    float distortionScale = getDistortionScale(p);\n    return (((((p * distortionScale) + lensCenter) + 1.0) * 0.5) * 0.5) + 0.25;\n\n    // float theta  = atan(p.y - lensCenter.y, p.x - lensCenter.x);\n    // float radius = length(p - lensCenter);\n    // radius = pow(radius, BarrelPower.x);\n    // p.x = lensCenter.x + radius * cos(theta);\n    // p.y = lensCenter.y + radius * sin(theta);\n    // return 0.5 * (p + 1.0);\n}\n \nvoid main(){\n    color = vec4(fract(theUV.xy), 0, 1);\n    color = texture( left_texture, distort(vec2((fract(theUV.x) * 2.0) - 1.0, theUV.y)) ).rgba;\n}"},
{"rift_vertex", "#version 330 core\n\n// Input vertex data, different for all executions of this shader.\nlayout(location = 0) in vec3 position;\n\n// Output data ; will be interpolated for each fragment.\nout vec2 theUV;\n\nvoid main(){\n  gl_Position = vec4(position, 1);\n  theUV = position.xy;\n}"},
{"vertex", "#version 330\n\nlayout (location = 0) in vec4 position;\nlayout (location = 1) in vec3 color;\n\nuniform mat4 projection;\nuniform mat4 modelview;\n\nin vec2 UV;\nout vec2 theUV;\n\nsmooth out vec3 theColor;\n\nvoid main()\n{\n    gl_Position = (projection * modelview * position);\n    theColor = color;\n    theUV = UV;\n}\n"},
};

#endif
