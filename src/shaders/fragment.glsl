#version 330

smooth in vec3 theColor;
smooth in vec4 theNormal;
smooth in vec4 thePosition;

uniform vec4 camera;

const float specular = .2;
const float diffuse = .3;
const float ambient = .5;

const float shine = 1;
const float surfaceSpec = 1;
const float surfaceDiff = 1;
const float surfaceAmb = 1;

layout(location = 0) out vec3 color;


///*
float dot(vec4 v, vec4 w) {
	return v.x*w.x + v.y*w.y + v.z*w.z - v.w*w.w;
}

float length(vec4 v) {
	return sqrt(dot(v,v));
}

vec4 normalize(vec4 v) {
	return v / length(v);
}

float angle(vec4 v, vec4 w) {
	return acos(dot(normalize(v),normalize(w)));
}

float dist(vec4 p1, vec4 p2) {
	return acosh(-dot(p1,p2));
}

vec4 expinv(vec4 basepoint, vec4 target) {
	float r = dist(basepoint, target);
	if(r==0) {
		return vec4(0,0,0,0);
	}
	vec4 u = target - cosh(r)*basepoint;
	return (r/sinh(r))*u;
}
//*/
void main() {
	//color = theColor;
	///*
	
	float intensity;
	
	vec4 norm = normalize(theNormal);
	vec4 lampPos = vec4(0, 10.0179, 0, 10.0677);
	vec4 lamp = expinv(thePosition, lampPos);
	vec4 lampNorm = normalize(lamp);
	
	vec4 optimal = 2*(dot(norm,lampNorm)*norm)-lampNorm;
	
	intensity = surfaceAmb*ambient + surfaceDiff*(dot(lamp,theNormal))*diffuse + surfaceSpec*pow((dot(optimal,camera)),shine)*specular;
	color = intensity*theColor;
	//*/
}