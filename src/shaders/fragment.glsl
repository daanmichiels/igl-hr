#version 330

smooth in vec3 theColor;
smooth in vec4 theNormal;
smooth in vec4 thePosition;

uniform vec4 camera;

const int specular = 1;
const int diffuse = 1;
const int ambient = 1;

const float shine = 0.5;
const float surfaceSpec = 0.3;
const float surfaceDiff = 0.7;
const float surfaceAmb = 0.5;

layout(location = 0) out vec3 color;

void main() {
	float intensity;
	
	vec4 norm = normalize(theNormal);
	vec4 lampPos = vec4(0, 10.0179, 0, 10.0677);
	vec4 lamp = expinv(thePosition, lampPos);
	float lampNorm = normalize(lamp);
	
	vec4 optimal = 2*(dot(norm,lampNorm)*norm)-lampNorm;
	
	intensity = surfaceAmb*ambient + surfaceDiff*(lamp*theNormal)*diffuse + surfaceSpec*pow((optimal*camera),shine)*specular;
	color = intensity*theColor;
}

double dot(vec4 v, vec4 w) {
	return v.x*w.x + v.y*w.y + v.z*w.z - v.w*w.w;
}

double length(vec4 v) {
	return sqrt(dot(v,v));
}

vec4 normalize(vec4 v) {
	return v / length(v);
}

double dist(vec4 p1, vec4 p2) {
	return acosh(-dot(p1,p2));
}

double angle(vec4 v, vec4 w) {
	return acos(dot(normalize(v),normalize(w)));
}

vec4 expinv(vec4 basepoint, vec4 target) {
	double r = dist(basepoint, target);
	if(r==0) {
		return vec4(0,0,0,0);
	}
	vec4 u = target - cosh(r)*basepoint;
	return (r/sinh(r))*u;
}