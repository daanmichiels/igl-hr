#version 330 core
 
in vec2 theUV; //TODO: rename this

out vec4 color;
 
uniform sampler2D texture;
uniform float aspect_ratio;
uniform vec2 lens_center; // in the range [-1,1]x[-1,1]
uniform vec4 distortion_params = vec4(1, 0.22, 0.24, 0);

const float PI = 3.1415926535;

// Convert from the uv coordinates in [0,1]x[0,1] to a more
// convenient coordinate system for distortion, where the lens_center
// corresponds to (0,0) and the x- and y-scales are the same
vec2 to_oculus_coordinates(vec2 p) {

    // from [0,1]x[0,1] to [-1,1]x[-1,1]
    p = vec2(-1.0, -1.0) + 2*p;

    // subtract lens center
    p = p - lens_center;

    // make x- and y-scales the same
    p.y = p.y / aspect_ratio;

    return p;
}

vec2 from_oculus_coordinates(vec2 p) {
    p.y = p.y * aspect_ratio;
    p = p + lens_center;
    p = vec2(0.5, 0.5) + 0.5*p;
    return p;
}

vec2 distort(vec2 p) {
    float r_squared = p.x * p.x + p.y * p.y;
    float factor = distortion_params[0] +
                   distortion_params[1] * r_squared +
                   distortion_params[2] * r_squared * r_squared +
                   distortion_params[3] * r_squared * r_squared * r_squared;
    return p * factor / 1.4;
}

void main() {
    vec2 new_uv = from_oculus_coordinates(distort(to_oculus_coordinates(theUV)));
    color = texture(texture, new_uv);
}

