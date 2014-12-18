#include "Material.h"
#include <stdlib.h>
#include "GL\glut.h"
Material::Material(){
	amGiven = diGiven = spGiven = shGiven = emGiven = coGiven = false;
}


Material::Material(f4 am, f4 di, f4 sp, float sh, f4 em, f3 co){
	ambient = am;
	diffuse = di;
	specular = sp;
	shininess[0] = sh;
	emission = em;
	color = co;
	amGiven = diGiven = spGiven = shGiven = emGiven = coGiven = true;
}
void Material::setAmbient(f4 am){
	ambient = am;
	amGiven = true;
}
void Material::setDiffuse(f4 di){
	diffuse = di;
	diGiven = true;
}
void Material::setSpecular(f4 sp){
	specular = sp;
	spGiven = true;
}
void Material::setShininess(float sh){
	shininess[0] = sh;
	shGiven = true;
}
void Material::setEmission(f4 em){
	emission = em;
	emGiven = true;
}
void Material::setColor(f3 co){
	color = co;
	coGiven = true;
}
void Material::apply(){
	if (amGiven)
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient.f);

	if (diGiven)
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse.f);
	
	if (spGiven)
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular.f);
	
	if (shGiven)
		glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	
	if (emGiven)
		glMaterialfv(GL_FRONT, GL_EMISSION, emission.f);
}