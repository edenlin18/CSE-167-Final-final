#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <Windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "glee.h"
#include <GL/glut.h>

#include "SOIL.h"
#include "Geode.h"

using namespace std;

class SkyBox {
	public:
	SkyBox();
	void init();
	void loadTexture(const unsigned int&);
	void render();
	~SkyBox();

	private:
	GLuint texture[6];
};

