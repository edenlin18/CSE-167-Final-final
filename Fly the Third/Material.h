#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Const.h"

using namespace control;

class Material{
public:
	Material();
	Material(f4 am, f4 di, f4 sp, float sh, f4 em, f3 co);
	void setAmbient(f4 am);
	void setDiffuse(f4 di);
	void setSpecular(f4 sp);
	void setShininess(float sh);
	void setEmission(f4 em);
	void setColor(f3 co);
	void apply();

private:
	f4 ambient, diffuse, specular, emission;
	float shininess[1];
	f3 color;
	bool amGiven, diGiven, spGiven, shGiven, emGiven, coGiven;
};



#endif