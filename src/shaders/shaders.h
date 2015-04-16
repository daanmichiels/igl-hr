#ifndef SHADERS_H
#define SHADERS_H

#include <string>
#include <map>

std::map<std::string, std::string> shader_sources = {
{"cross_fragment", "#version 330\n\nsmooth in vec3 theColor;\nin vec2 location;\n\nlayout(location = 0) out vec3 color;\n\nvoid main() {\n    color = (theColor);\n    if (location.x > 0 && location.y > 0) {\n      color = vec3(0,0,0);\n    }\n}\n"},
{"cross_vertex", "#version 330\n\nlayout (location = 0) in vec4 position;\nlayout (location = 1) in vec3 color;\n\nuniform mat4 projection;\nuniform mat4 modelview;\n\nsmooth out vec3 theColor;\nout vec2 location;\n\nvoid main()\n{\n    gl_Position = (projection * modelview * position);\n    location = gl_Position.xy;\n    theColor = color;\n}\n"},
{"fragment", "#version 330\n\nsmooth in vec3 theColor;\n\nlayout(location = 0) out vec3 color;\n\nvoid main() {\n    color = (theColor);\n}\n"},
{"fragment_offset", "#version 330\n\nsmooth in vec3 theColor;\nuniform vec3 theOffset;\n\nlayout(location = 0) out vec3 color;\n\nvoid main() {\n    color = clamp(theColor + theOffset, 0.0, 1.0);\n}\n"},
{"rift_fragment", "#version 330 core\n \nin vec2 theUV; //TODO: rename this\n\nout vec4 color;\n \nuniform sampler2D the_texture;\nuniform float aspect_ratio;\nuniform vec2 lens_center; // in the range [-1,1]x[-1,1]\nuniform vec4 distortion_params = vec4(1, 0.22, 0.24, 0);\n\nconst float PI = 3.1415926535;\n\n// Convert from the uv coordinates in [0,1]x[0,1] to a more\n// convenient coordinate system for distortion, where the lens_center\n// corresponds to (0,0) and the x- and y-scales are the same\nvec2 to_oculus_coordinates(vec2 p) {\n\n    // from [0,1]x[0,1] to [-1,1]x[-1,1]\n    p = vec2(-1.0, -1.0) + 2*p;\n\n    // subtract lens center\n    p = p - lens_center;\n\n    // make x- and y-scales the same\n    p.y = p.y / aspect_ratio;\n\n    return p;\n}\n\nvec2 from_oculus_coordinates(vec2 p) {\n    p.y = p.y * aspect_ratio;\n    p = p + lens_center;\n    p = vec2(0.5, 0.5) + 0.5*p;\n    return p;\n}\n\nvec2 distort(vec2 p) {\n    float r_squared = p.x * p.x + p.y * p.y;\n    float factor = distortion_params[0] +\n                   distortion_params[1] * r_squared +\n                   distortion_params[2] * r_squared * r_squared +\n                   distortion_params[3] * r_squared * r_squared * r_squared;\n    return p * factor / 1.4;\n}\n\nvoid main() {\n    vec2 new_uv = from_oculus_coordinates(distort(to_oculus_coordinates(theUV)));\n    color = texture(the_texture, new_uv);\n}\n\n"},
{"rift_vertex", "#version 330 core\n\nlayout(location = 0) in vec2 position;\nlayout(location = 1) in vec2 uv;\n\nout vec2 theUV;\n\nvoid main(){\n  gl_Position = vec4(position, 0, 1);\n  theUV = uv;\n}\n"},
{"vertex", "#version 330\n\nlayout (location = 0) in vec4 position;\nlayout (location = 1) in vec3 color;\n\nuniform mat4 projection;\nuniform mat4 modelview;\n\nsmooth out vec3 theColor;\n\nvoid main()\n{\n    gl_Position = (projection * modelview * position);\n    theColor = color;\n}\n"},
};

#endif
