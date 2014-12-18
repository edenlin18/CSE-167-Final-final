#include "Building.h"

Building::Building(GLuint buildingChoice, GLuint roofChoice, double r, double g, double b, GLuint* texture, GLdouble xScaling, GLdouble yScaling, GLdouble zScaling, Matrix4d rootC) {
	Matrix4d initT;
	
	initT.makeTranslate(0.0, HALF_BUILDING_SIZE, 0.0);
	rootC = rootC * initT;
	root = new MatrixTransform(rootC);
	root->addChild(new Cube(texture, buildingChoice, roofChoice, xScaling, yScaling, zScaling, BUILDING_SIZE, Vector3d(r, g, b), draw::SOLID));
}

MatrixTransform* Building::getRoot() {
	return this->root;
}

Building::~Building() {
}
