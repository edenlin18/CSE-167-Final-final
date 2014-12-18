#include "SkyBox.h"

SkyBox::SkyBox() {

}

Bs SkyBox::update() {
	return Bs();
}

void SkyBox::init() {
	// Create ID for texture
	glGenTextures(6, &texture[0]);

	int width, height;
	/*
	char* filenames[6] = { "./skyBox/Skybox_Water222_right.jpg", "./skyBox/Skybox_Water222_left.jpg",
		"./skyBox/Skybox_Water222_top.jpg", "./skyBox/Skybox_Water222_base.jpg", "./skyBox/Skybox_Water222_back.jpg",
		"./skyBox/Skybox_Water222_front.jpg" };
	*/

	/*
	char* filenames[6] = { "./skyBox/TropicalSunnyDayRight2048.png", "./skyBox/TropicalSunnyDayLeft2048.png",
		"./skyBox/TropicalSunnyDayUp2048.png", "./skyBox/TropicalSunnyDayDown2048.png", "./skyBox/TropicalSunnyDayBack2048.png",
		"./skyBox/TropicalSunnyDayFront2048.png" };
	*/

	///*
	char* filenames[6] = { "./skyBox/TropicalSunnyDayLeft2048.png", "./skyBox/TropicalSunnyDayRight2048.png",
		"./skyBox/TropicalSunnyDayUp2048.png", "./skyBox/TropicalSunnyDayDown2048.png",
		"./skyBox/TropicalSunnyDayBack2048.png", "./skyBox/TropicalSunnyDayFront2048.png" };
	//*/

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

		// Use bilinear interpolation:
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
}

// load image file into texture object
void SkyBox::loadTexture(const unsigned int& opt) {
	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, texture[opt]);
}

void SkyBox::render() {
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	float SkyBoxVertices [] =
	{	// x, y, z, x, y, z, x, y, z, x, y, z
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, // +X
		-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, // -X
		-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, // +Y
		-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, // -Y
		1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // +Z
		-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f  // -Z
	};

	unsigned int ratio = 100;

	for (unsigned int counter = 0; counter < 6; counter++) {
		if (counter == 3)
			continue;
		unsigned int row = 12 * counter;
		loadTexture(counter);
		glBegin(GL_QUADS);
		glColor3f(1, 1, 1);
		glTexCoord2f(0, 0);
		glVertex3f(ratio * SkyBoxVertices[row], ratio * SkyBoxVertices[row + 1], ratio * SkyBoxVertices[row + 2]);

		glTexCoord2f(1, 0);
		glVertex3f(ratio * SkyBoxVertices[row + 3], ratio * SkyBoxVertices[row + 4], ratio * SkyBoxVertices[row + 5]);

		glTexCoord2f(1, 1);
		glVertex3f(ratio * SkyBoxVertices[row + 6], ratio * SkyBoxVertices[row + 7], ratio * SkyBoxVertices[row + 8]);

		glTexCoord2f(0, 1);
		glVertex3f(ratio * SkyBoxVertices[row + 9], ratio * SkyBoxVertices[row + 10], ratio * SkyBoxVertices[row + 11]);
		glEnd();
	}
}

SkyBox::~SkyBox() {
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, image);
}
