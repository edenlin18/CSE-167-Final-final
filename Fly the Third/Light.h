#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Const.h"

using namespace control;

class Light{

public:
	Light();
	Light(f4 am, f4 di, f4 sp, f4 po);
	void setAmbient(f4 am);
	void setDiffuse(f4 di);
	void setSpecular(f4 sp);
	void setPosition(f4 po);
	void setSpotDirection(f3 sp_di);
	void setExponent(float exp);
	void setCutOff(float cf);
	void setAttenuation(float c, float l, float q);
	float getCutOff();
	void isSpotLight(bool b);
	void apply();

private:
	f4 ambient, diffuse, specular, position;
	f3 spot_direction;
	float exponent, cut_off, c_attenuation, l_attenuation, q_attenuation;
	static int lightCount;
	bool amGiven, diGiven, spGiven, poGiven;
	bool spot;
	int id;
};

#endif