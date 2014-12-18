#include "Light.h"
#include <stdlib.h>
#include "GL\glut.h"

#include <iostream>

int Light::lightCount = -1;

Light::Light(){
	ambient.f[0] = 0.0;
	ambient.f[1] = 0.0;
	ambient.f[2] = 0.0;
	ambient.f[3] = 1.0;

	diffuse.f[0] = 1.0;
	diffuse.f[1] = 1.0;
	diffuse.f[2] = 1.0;
	diffuse.f[3] = 1.0;

	specular.f[0] = 1.0;
	specular.f[1] = 1.0;
	specular.f[2] = 1.0;
	specular.f[3] = 1.0;

	position.f[0] = 0.0;
	position.f[1] = 0.0;
	position.f[2] = 1.0;
	position.f[3] = 0.0;

	cut_off = 180;
	spot_direction.f[0] = 0.0;
	spot_direction.f[1] = 0.0;
	spot_direction.f[2] = -1.0;
	c_attenuation = 1.0;
	l_attenuation = 0.0;
	q_attenuation = 0.0;
	spot = false;
	lightCount++;
	id = lightCount;
}

Light::Light(f4 am, f4 di, f4 sp, f4 po){
	ambient = am;
	diffuse = di;
	specular = sp;
	position = po;
	cut_off = 180;
	spot_direction.f[0] = 0.0;
	spot_direction.f[1] = 0.0;
	spot_direction.f[2] = -1.0;
	c_attenuation = 1.0;
	l_attenuation = 0.0;
	q_attenuation = 0.0;
	spot = false;
	lightCount++;
	id = lightCount;
}

void Light::setAmbient(f4 am){
	ambient = am;
}
void Light::setDiffuse(f4 di){
	diffuse = di;
}
void Light::setSpecular(f4 sp){
	specular = sp;
}
void Light::setPosition(f4 po){
	position = po;
}
void Light::setSpotDirection(f3 sp_di){
	spot_direction = sp_di;
	spot = true;
}
void Light::setExponent(float exp){
	exponent = exp;
}
void Light::setCutOff(float cf){
	cut_off = cf;
	spot = true;
}

void Light::setAttenuation(float c, float l, float q){
	c_attenuation = c;
	l_attenuation = l;
	q_attenuation = q;
}

void Light::isSpotLight(bool b){
	spot = b;
}

void Light::apply(){
	
	glLightfv(GL_LIGHT0 + id, GL_AMBIENT, ambient.f);
	glLightfv(GL_LIGHT0 + id, GL_DIFFUSE, diffuse.f);
	glLightfv(GL_LIGHT0 + id, GL_SPECULAR, specular.f);
	glLightfv(GL_LIGHT0 + id, GL_POSITION, position.f);
	glLightf(GL_LIGHT0 + id, GL_CONSTANT_ATTENUATION, c_attenuation);
	glLightf(GL_LIGHT0 + id, GL_LINEAR_ATTENUATION, l_attenuation);
	glLightf(GL_LIGHT0 + id, GL_QUADRATIC_ATTENUATION, q_attenuation);	
	if (spot){
		glLightf(GL_LIGHT0 + id, GL_SPOT_EXPONENT, exponent);
		glLightf(GL_LIGHT0 + id, GL_SPOT_CUTOFF, cut_off);
		glLightfv(GL_LIGHT0 + id, GL_SPOT_DIRECTION, spot_direction.f);
	}
	glEnable(GL_LIGHT0 + id);
}

float Light::getCutOff(){
	return cut_off;
}