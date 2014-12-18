#pragma once

#include <math.h>
#include <iostream>

#include "main.h"

#include "Matrix4d.h"
#include "shader.h"

#include "Scene.h"

using namespace std;

// OpenGL output window related routines
class Window{
public:
	// Window size
	static GLfloat width, height;
	static Shader* shader;

	// Frame Buffer Object ID
	static GLuint fboID;

	// Members
	static Scene* scene;

	// Helpers
	static void initialize();
	static void initializeFBO();
	static void initializeShader();
	static void initializeTexture();
	static void update();

	static void setupMatrices(float position_x, float position_y, float position_z, float lookAt_x, float lookAt_y, float lookAt_z);
	static void setTextureMatrix();
	static void startTranslate(float x, float y, float z);
	static void endTranslate();
	static void draw();

	// Shadow Map Functions
	static void drawSM();
	static void firstPassSM();
	static void secondPassSM();
	static void thirdPassSM();

	// Regular render
	static void drawR();

	static void idleCallback(void);
	static void reshapeCallback(int, int);
	static void displayCallback(void);

	// Process key events
	static void processKeyboard(unsigned char, int, int);
	static void processSpecial(int, int, int);

	static void debug(const string& comment);
};
