#include "Window.h"

GLfloat Window::width = 640.0;
GLfloat Window::height = 480.0;

GLuint Window::fboID;

Shader* Window::shader;

Scene* Window::scene;

bool DEBUG = false;
bool debugSM = false;

bool SM_on = true;
bool update_on = false;

const bool PCF = true;

const int X = 0;
const int Y = 1;
const int Z = 2;

//gluPerspective(45, width / height, 10, 40000);

GLuint shading;

enum SHADING{
	LIGHT = 0, TEXTURE, SHADOW
};

SHADING mode = LIGHT;

enum TEXTURE{
	CUBE, FLOOR, TEXTURE_COUNT
};

GLuint texture[TEXTURE_COUNT];

// Shadow Map
namespace SM{
	const int RATIO = 2;

	const GLfloat width = 1280.0;
	const GLfloat height = 960.0;

	GLhandleARB shaderID;
	GLuint uniform;
	GLuint uniformX;
	GLuint uniformY;

	// Depth texture
	GLuint texture;
}

// Light
namespace L{
	float position[3] = { 0, 75, 60 };
	float lookAt[3] = { 0, 0, 0 };
	const float MOVEMENT = 30.0;

	const GLdouble FOVY = 60;
	const GLdouble NEARZ = 10;
	const GLdouble FARZ = 40000;

	// View & Project Matrix
	Matrix4d VM;
	Matrix4d PM;

	//Light position
	//float p_light[3] = {3,20,0};

	//Light lookAt
	//float l_light[3] = {0,0,-5};

	//Light mouvement circle radius
	//float light_mvnt = 30.0f;
}

// Camera
namespace C{
	//float position[3] = { 5, 40, 80 };
	float position[3] = { 1, 40, 80 };
	//float lookAt[3] = { 2, 0, -10 };
	float lookAt[3] = { 0, 0, 0 };

	const GLdouble FOVY = 45;
	const GLdouble NEARZ = 10;
	const GLdouble FARZ = 40000;

	// View & Project Matrix
	Matrix4d VM;
	Matrix4d PM;

	//Camera position
	//float p_camera[3] = {32,20,0};

	//Camera lookAt
	//float l_camera[3] = {2,0,-10};
}

void Window::initialize(){

	// Framebuffer Object + Shader
	initializeFBO();
	initializeShader();

	// Scene
	scene = new Scene();
	scene->initialize();

	// Save Light & Camera matrices
	glPushMatrix();

	// Light Projection Matrix
	glLoadIdentity();
	gluPerspective(L::FOVY, width / height, L::NEARZ, L::FARZ);
	glGetDoublev(GL_MODELVIEW_MATRIX, L::PM.getPointer());

	// Light View Matrix
	glLoadIdentity();
	gluLookAt(
		L::position[X], L::position[Y], L::position[Z],
		L::lookAt[X], L::lookAt[Y], L::lookAt[Z],
		0, 1, 0
		);
	glGetDoublev(GL_MODELVIEW_MATRIX, L::VM.getPointer());

	// Camera Projection Matrix
	glLoadIdentity();
	gluPerspective(C::FOVY, width / height, C::NEARZ, C::FARZ);
	glGetDoublev(GL_MODELVIEW_MATRIX, C::PM.getPointer());

	// Camera View Matrix
	glLoadIdentity();
	gluLookAt(
		C::position[X], C::position[Y], C::position[Z],
		C::lookAt[X], C::lookAt[Y], C::lookAt[Z],
		0, 1, 0
		);
	glGetDoublev(GL_MODELVIEW_MATRIX, C::VM.getPointer());

	glPopMatrix();

	debug("initialize()");
}

void Window::initializeFBO(){
	// This is important, if not here, FBO's depthbuffer won't be populated.
	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 1.0f);

	glEnable(GL_CULL_FACE);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Generate Frame Buffer Object;
	//int shadowMapWidth = width * SM::RATIO;
	//int shadowMapHeight = height * SM::RATIO;

	// Holds errors if any
	GLenum FBOstatus;

	// Try to use a texture depth component
	glGenTextures(1, &SM::texture);
	glBindTexture(GL_TEXTURE_2D, SM::texture);

	// GL_LINEAR does not make sense for depth texture. However, next tutorial shows usage of GL_LINEAR and PCF
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Remove artefact on the edges of the shadowmap
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// PCF
	if (PCF){
		// This is to allow usage of shadow2DProj function in the shader
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
	}

	// No need to force GL_DEPTH_COMPONENT24, drivers usually give you the max precision if available 
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SM::width, SM::height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// create a framebuffer object
	glGenFramebuffersEXT(1, &fboID);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);

	// Instruct openGL that we won't bind a color texture with the currently binded FBO
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// attach the texture to FBO depth attachment point
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, SM::texture, 0);

	// check FBO status
	FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (FBOstatus != GL_FRAMEBUFFER_COMPLETE_EXT)
		printf("GL_FRAMEBUFFER_COMPLETE_EXT failed, CANNOT use FBO\n");

	// switch back to window-system-provided framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

// Initialize shaders and uniforms
void Window::initializeShader(){
	// Load shaders

	if (PCF){
		shader = new Shader("shader/ShadowPCF.vert", "shader/ShadowPCF.frag");
	}
	else{
		shader = new Shader("shader/Shadow.vert", "shader/Shadow.frag");
	}
	
	SM::shaderID = shader->getPid();

	SM::uniform = glGetUniformLocationARB(SM::shaderID, "ShadowMap");
	shading = glGetUniformLocationARB(SM::shaderID, "Shading");

	if (PCF){
		SM::uniformX = glGetUniformLocationARB(SM::shaderID, "xPixelOffset");
		SM::uniformY = glGetUniformLocationARB(SM::shaderID, "yPixelOffset");
	}

	shader->bind();
	glUniform1iARB(glGetUniformLocation(SM::shaderID, "Texture"), 0);
	shader->unbind();
}

void Window::initializeTexture(){
	texture[CUBE] = SOIL_load_OGL_texture(
		"resource/cube.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

	texture[FLOOR] = SOIL_load_OGL_texture(
		"resource/floor.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

	if (texture[CUBE] == 0){
		cerr << "CUBE NOT FOUND" << endl;
	}
	if (texture[FLOOR] == 0){
		cerr << "FLOOR NOT FOUND" << endl;
	}
}

// Updates window
void Window::update(){
	// Rotate and rotate!
	L::position[X] = L::MOVEMENT * cos(glutGet(GLUT_ELAPSED_TIME) / 1000.0);
	L::position[Z] = L::MOVEMENT * sin(glutGet(GLUT_ELAPSED_TIME) / 1000.0);

	// Update light view matrix
	glPushMatrix();
	glLoadIdentity();
	gluLookAt(
		L::position[X], L::position[Y], L::position[Z],
		L::lookAt[X], L::lookAt[Y], L::lookAt[Z],
		0, 1, 0
		);
	glGetDoublev(GL_MODELVIEW_MATRIX, L::VM.getPointer());
	glPopMatrix();

	glLightfv(GL_LIGHT0, GL_POSITION, L::position);
}

void Window::setTextureMatrix(){
	static double modelView[16];
	static double projection[16];

	// Moving from unit cube [-1,1] to [0,1]  
	const GLdouble bias[16] = {
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	};

	// Grab modelview and transformation matrices
	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);

	glLoadIdentity();
	glLoadMatrixd(bias);

	// concatating all matrice into one.
	glMultMatrixd(projection);
	glMultMatrixd(modelView);

	// Go back to normal matrix mode
	glMatrixMode(GL_MODELVIEW);
}

///*
void Window::draw(){

	debug("draw() begin");

	glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	scene->draw(Matrix4d());
	//glPopMatrix();

	debug("draw()");
}
//*/


// First pass, light POV
void Window::firstPassSM(){

	//First step: Render from the light POV to a FBO, story depth values only
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);	//Rendering offscreen

	//Using the fixed pipeline to render to the depthbuffer
	glUseProgramObjectARB(0);

	// In the case we render the shadowmap to a higher resolution, the viewport must be modified accordingly.
	//glViewport(0, 0, width * SM::RATIO, height * SM::RATIO);

	// ADDITION
	glViewport(0, 0, SM::width, SM::height);

	// Clear previous frame values
	glClear(GL_DEPTH_BUFFER_BIT);

	//Disable color rendering, we only want to write to the Z-Buffer
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	/*
	//setupMatrices(p_light[0], p_light[1], p_light[2], l_light[0], l_light[1], l_light[2]);
	setupMatrices(
		L::position[X], L::position[Y], L::position[Z],
		L::lookAt[X], L::lookAt[Y], L::lookAt[Z]
		);
	*/

	///*
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(L::PM.getPointer());
	//*/

	// Light View Matrix, update() changes Light's position
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(L::VM.getPointer());

	// Culling switching, rendering only backface, this is done to avoid self-shadowing
	glCullFace(GL_FRONT);

	// First render
	draw();


	//Save modelview/projection matrice into texture7, also add a bias
	setTextureMatrix();

	debug("firstPassSM()");
}

// Second pass, camera POV
void Window::secondPassSM(){
}

void Window::thirdPassSM(){

	// Now rendering from the camera POV, using the FBO to generate shadows
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	glViewport(0, 0, width, height);

	//Enabling color write (previously disabled for light POV z-buffer rendering)
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	// Clear previous frame values
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Using the shadow shader
	//glUseProgramObjectARB(shadowShaderId);
	shader->bind();

	glUniform1iARB(SM::uniform, 7);

	if (PCF){
		glUniform1fARB(SM::uniformX, 1.0 / SM::width);
		glUniform1fARB(SM::uniformY, 1.0 / SM::height);
	}

	glActiveTextureARB(GL_TEXTURE7);

	// Bind & enable shadow map texture
	glBindTexture(GL_TEXTURE_2D, SM::texture);

	/*
	//setupMatrices(p_camera[0], p_camera[1], p_camera[2], l_camera[0], l_camera[1], l_camera[2]);
	setupMatrices(
	C::position[X], C::position[Y], C::position[Z],
	C::lookAt[X], C::lookAt[Y], C::lookAt[Z]
	);
	*/

	// Render from camera POV
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(C::PM.getPointer());

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(C::VM.getPointer());

	glCullFace(GL_BACK);

	// ADDITION, Lighting, these are sent to the shader
	static GLfloat ambient[] = { 1, 1, 1, 1 };
	static GLfloat diffuse[] = { 1, 1, 1, 1 };
	static GLfloat specular[] = { 1, 1, 1, 1 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, L::position);

	glUniform1i(shading, mode);

	/*
	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D, texture[CUBE]);
	*/

	// Second render
	draw();

	shader->unbind();

	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D, 0);

	debug("thirdPassSM()");
}

void Window::drawSM(){
	firstPassSM();
	secondPassSM();
	thirdPassSM();

	// DEBUG only. this piece of code draw the depth buffer onscreen
	if (debugSM){
		glUseProgramObjectARB(0);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-width / 2, width / 2, -height / 2, height / 2, 1, 20);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glColor4f(1, 1, 1, 1);
		glActiveTextureARB(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, SM::texture);
		glEnable(GL_TEXTURE_2D);
		glTranslated(0, 0, -1);
		glBegin(GL_QUADS);
		glTexCoord2d(0, 0); glVertex3f(0, 0, 0);
		glTexCoord2d(1, 0); glVertex3f(width / 2, 0, 0);
		glTexCoord2d(1, 1); glVertex3f(width / 2, height / 2, 0);
		glTexCoord2d(0, 1); glVertex3f(0, height / 2, 0);

		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
}

// Regular render
void Window::drawR(){

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// ADDITION
	glViewport(0, 0, width, height);

	// Clear previous frame values
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(C::PM.getPointer());

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(C::VM.getPointer());

	draw();

	glPopMatrix();

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
}

void Window::displayCallback(){
	
	glMatrixMode(GL_MODELVIEW);

	if (update_on){
		update();
	}


	drawSM();

	// LIGHT AND GROUND
	glPushMatrix();

	glBegin(GL_LINES);

	// LIGHT
	glColor3f(1.0, 1.0, 0.5);
	glVertex3f(L::position[X], 0.0, L::position[Z]);
	glVertex3f(L::position[X], 10000.0, L::position[Z]);
	glVertex3f(L::position[X], 0.0, L::position[Z]);
	glVertex3f(L::position[X], -10000.0, L::position[Z]);

	glVertex3f(L::position[X], L::position[Y], 0.0);
	glVertex3f(L::position[X], L::position[Y], 10000.0);
	glVertex3f(L::position[X], L::position[Y], 0.0);
	glVertex3f(L::position[X], L::position[Y], -10000.0);

	glVertex3f(0.0, L::position[Y], L::position[Z]);
	glVertex3f(10000.0, L::position[Y], L::position[Z]);
	glVertex3f(0.0, L::position[Y], L::position[Z]);
	glVertex3f(-10000.0, L::position[Y], L::position[Z]);

	// GROUND
	glColor3f(0, 0, 0);
	glVertex3f(0, 0.0, 0);
	glVertex3f(0, 10000.0, 0);
	glVertex3f(0, 0.0, 0);
	glVertex3f(0, -10000.0, 0);

	glColor3f(1, 1, 1);
	glEnd();

	//glTranslatef(lightPosition[X], lightPosition[Y], lightPosition[Z]);
	//glutSolidSphere(0.1, 10, 10);

	glPopMatrix();

	glutSwapBuffers();
	

}

void Window::debug(const string& comment) {

	if (DEBUG) {
		GLuint error = glGetError();
		if (error != GL_NO_ERROR) {
			cerr << hex << error << " AT " << comment << endl;
			system("PAUSE");
		}
	}

}

void Window::reshapeCallback(int w, int h){
	//Save new window size
	width = w, height = h;

	///*
	//Update the camera's projection matrix
	glPushMatrix();
	glLoadIdentity();
	gluPerspective(C::FOVY, (float)width / height, C::NEARZ, C::FARZ);
	glGetDoublev(GL_MODELVIEW_MATRIX, C::PM.getPointer());
	glPopMatrix();
	//*/
}

void Window::idleCallback(){
	displayCallback();
}

void Window::processKeyboard(unsigned char key, int x, int y){
	switch (key){
	case 27: // ESC
		exit(0);
		break;
	}
}

void Window::processSpecial(int key, int x, int y){

	GLdouble move = 1.0;

	switch (key){
	case GLUT_KEY_F1:
		mode = LIGHT;
		break;
	case GLUT_KEY_F2: // Toggle shadow
		mode = TEXTURE;
		break;
	case GLUT_KEY_F3:
		mode = SHADOW;
		break;
	case GLUT_KEY_UP:
		L::position[Z] -= move;
		break;
	case GLUT_KEY_DOWN:
		L::position[Z] += move;
		break;
	case GLUT_KEY_LEFT:
		L::position[X] -= move;
		break;
	case GLUT_KEY_RIGHT:
		L::position[X] += move;
		break;
	case GLUT_KEY_PAGE_UP:
		L::position[Y] += move;
		break;
	case GLUT_KEY_PAGE_DOWN:
		L::position[Y] -= move;
		break;
	}

	// Update light view matrix
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		L::position[X], L::position[Y], L::position[Z],
		L::lookAt[X], L::lookAt[Y], L::lookAt[Z],
		0, 1, 0
		);
	glGetDoublev(GL_MODELVIEW_MATRIX, L::VM.getPointer());
	glPopMatrix();

	glLightfv(GL_LIGHT0, GL_POSITION, L::position);
}