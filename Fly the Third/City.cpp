#include "City.h"

City::City(MatrixTransform* mt) {
	Matrix4d translationSetter;
	Matrix4d ScalingSetter;
	BuildingConstructor* bConstructor = NULL;
	GLdouble maxH = 0.0;
	GLdouble minH = 0.0;
	root = new MatrixTransform(Matrix4d());
	int layerCount = 0;
	int choice = 0;

	/*
	maxH = ((double) rand() * (MEDIUM_BUILDING_MAX_HEIGHT - MEDIUM_BUILDING_MIN_HEIGHT)) / (double) RAND_MAX + MEDIUM_BUILDING_MIN_HEIGHT;
	minH = ((double) rand() * (maxH - MEDIUM_BUILDING_MIN_HEIGHT)) / (double) RAND_MAX + MEDIUM_BUILDING_MIN_HEIGHT;
	layerCount = (rand() * (3 - 1)) / RAND_MAX + 1;
	choice = 0;
	bConstructor = new BuildingConstructor(texture, translationSetter, maxH, minH, layerCount, choice);
	root->addChild(bConstructor->getRoot());*/

	/*Turtle3D* turtle = new Turtle3D("ABC-D+E", "ABCDE", 20, 20, 0.8);
	turtle->addRule('A', "FFFFFFF[-\AE]F[+&+AE]FFF", 1.0);
	turtle->addRule('B', "^^^FF", 0.3);
	turtle->addRule('B', "FF&&&FF", 0.3);
	turtle->addRule('B', "+F\\FF", 0.4);
	turtle->addRule('C', "F[/+CE]F[^AE]F", 1.0);
	turtle->addRule('D', "FF&F", 1.0);
	MatrixTransform* mt = turtle->generate(5);*/


	for (double z = -50.0; z < 50.0; z += 5.0) {
		for (double x = -50.0; x < 50.0; x += 5.0) {
			translationSetter.makeTranslate(x, 0.0, z);
			if (-17.0 < x && x < 17.0) {
				maxH = ((double) rand() * (MEDIUM_BUILDING_MAX_HEIGHT - MEDIUM_BUILDING_MIN_HEIGHT)) / (double) RAND_MAX + MEDIUM_BUILDING_MIN_HEIGHT;
				minH = ((double) rand() * (maxH - MEDIUM_BUILDING_MIN_HEIGHT)) / (double) RAND_MAX + MEDIUM_BUILDING_MIN_HEIGHT;
				layerCount = (rand() * (3 - 1)) / RAND_MAX + 1;
				choice = 0;
			}
			else if (x <= -17.0) {
				maxH = ((double) rand() * (TALL_BUILDING_MAX_HEIGHT - TALL_BUILDING_MIN_HEIGHT)) / (double) RAND_MAX + TALL_BUILDING_MIN_HEIGHT;
				minH = ((double) rand() * (maxH - TALL_BUILDING_MIN_HEIGHT)) / (double) RAND_MAX + TALL_BUILDING_MIN_HEIGHT;
				layerCount = (rand() * (5 - 3)) / RAND_MAX + 3;
				choice = 1;
			}
			else if (x >= 17.0 && x <= 50.0) {
				maxH = ((double) rand() * (TALL_BUILDING_MAX_HEIGHT - TALL_BUILDING_MIN_HEIGHT)) / (double) RAND_MAX + TALL_BUILDING_MIN_HEIGHT;
				minH = ((double) rand() * (maxH - TALL_BUILDING_MIN_HEIGHT)) / (double) RAND_MAX + TALL_BUILDING_MIN_HEIGHT;
				layerCount = (rand() * (7 - 3)) / RAND_MAX + 3;
				choice = 2;
			}
			else if (x > 50.0 && x < 56.0) {
				Matrix4d t;
				t.makeTranslate(x, -1, z);
				MatrixTransform* translation = new MatrixTransform(t);
				//tt = new MatrixTransform(t);
				//root->addChild(rotate_mt);
				//rotate_mt->addChild(scaling_mt);
				//scaling_mt->addChild(tt);
				//tt->addChild(mt);
				root->addChild(translation);
				translation->addChild(mt);
			}

			bConstructor = new BuildingConstructor(texture, translationSetter, maxH, minH, layerCount, choice);
			root->addChild(bConstructor->getRoot());
		}
	}

	ScalingSetter.makeScale(50.0, 0.0, 50.0);
	MatrixTransform* groundTransform = new MatrixTransform(ScalingSetter);
	root->addChild(groundTransform);
	groundTransform->addChild(new Quad());
	init();
}

void City::init() {


	// Create ID for texture
	glGenTextures(6, &texture[0]);

	char* filenames[6] = { "./building_texture/building5.jpg", "./building_texture/pixelcity_windows7.jpg", "./building_texture/building3.jpg",
		"./building_texture/building6.jpg", "./building_texture/roof1.jpg", "./building_texture/roof2.jpg" };

	for (unsigned int counter = 0; counter < 6; ++counter) {

		texture[counter] = SOIL_load_OGL_texture
			(
			filenames[counter],
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
			);

		if (texture[counter] == 0) {
			std::cerr << "FAIL AT " << counter << std::endl;
		}

		// Make sure no bytes are padded:
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// Select GL_MODULATE to mix texture with polygon color for shading:
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// Use bilinear interpolation:
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	}
}

MatrixTransform* City::getRoot() {
	return this->root;
}

City::~City() {
}
