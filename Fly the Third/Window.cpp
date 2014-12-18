#include "Window.h"

GLfloat Window::width = 640.0;
GLfloat Window::height = 480.0;

GLuint Window::fboID;

Shader* Window::shader;

Scene* Window::scene;

bool DEBUG = true;
bool debugSM = false;

bool SM_on = true;
bool update_on = false;

bool PCF = false;

const int X = 0;
const int Y = 1;
const int Z = 2;

enum TEXTURE{
	CUBE, FLOOR, TEXTURE_COUNT
};

GLuint texture[TEXTURE_COUNT];

const char* VERTEXSHADER = "shader/Shading.vert";
const char* FRAGMENTSHADER = "shader/Shading.frag";

enum {
	LIGHT = 0, 
	TEXTURE = 1, 
	SHADOW = 2
};

GLuint shading;
int mode = LIGHT;

// Texture
namespace T{
	GLuint uniform;
	GLuint enable;
	GLuint disable;
}

// Shadow Map
namespace SM{
	const int RATIO = 2;

	const GLfloat WIDTH = 2048.0;
	const GLfloat HEIGHT = 2048.0;

	GLhandleARB shaderID;
	GLuint uniform;
	GLuint uniformX;
	GLuint uniformY;

	// Depth texture
	GLuint texture;

	GLuint matrix;
}

// Light
namespace L{
	float position[3] = { 0, 10, 20 };
	float lookAt[3] = { 0, 0, 0 };
	const float MOVEMENT = 30.0;

	//gluPerspective(35.0f, 1.0f, 4.0f, 16.0f);
	//static vec3 LightPosition = vec3(0.0f, 2.5f, 5.0f);

	const GLdouble FOVY = 90;
	const GLdouble AR = 1.0;
	const GLdouble NEARZ = 1.0;
	const GLdouble FARZ = 40000.0;
	
	const float ambient[] = { 0.25, 0.25, 0.25, 1.0 };
	const float diffuse[] = { 0.75, 0.75, 0.75, 1.0 };
	const float specular[] = { 1, 1, 1, 1 };


	// View & Project Matrix
	Matrix4d VM;
	Matrix4d PM;
}

// Camera
namespace C{
	//float position[3] = { 5, 40, 80 };
	float position[3] = { 5, 20, 40 };
	//float lookAt[3] = { 2, 0, -10 };
	float lookAt[3] = { 0, 0, 0 };

	//Camera.Look(vec3(1.75f, 1.75f, 5.0f), vec3(0.0f, 0.0f, 0.0f));

	const GLdouble FOVY = 60;
	const GLdouble NEARZ = 1;
	const GLdouble FARZ = 40000.0;

	//perspective(45.0f, (float)Width / (float)Height, 0.125f, 512.0f);

	// View & Project Matrix
	Matrix4d VM;
	Matrix4d IVM;
	Matrix4d PM;
}

void Window::initialize(){

	initializeFBO();
	initializeShader();
	initializeTexture();
	initializeMatrix();

	// SCENE
	scene = new Scene();
	scene->initialize();

	// LIGHTING
	glLightfv(GL_LIGHT0, GL_POSITION, L::position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, L::ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, L::diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, L::specular);

	debug("initialize()");
}

void Window::initializeFBO(){

	glGenTextures(1, &SM::texture);
	glBindTexture(GL_TEXTURE_2D, SM::texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	// glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);

	if (PCF){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, SM::WIDTH, SM::HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffersEXT(1, &fboID);

	debug("initializeFBO()");
}

// Initialize shaders and uniforms
void Window::initializeShader(){

	shader = new Shader(VERTEXSHADER, FRAGMENTSHADER);

	SM::shaderID = shader->getPid();

	shading = glGetUniformLocationARB(SM::shaderID, "Shading");
	SM::matrix = glGetUniformLocationARB(SM::shaderID, "ShadowMatrix");
	SM::uniformX = glGetUniformLocationARB(SM::shaderID, "OffsetX");
	SM::uniformY = glGetUniformLocationARB(SM::shaderID, "OffsetY");

	shader->bind();

	glUniform1iARB(glGetUniformLocation(SM::shaderID, "Texture"), 0);
	glUniform1iARB(glGetUniformLocation(SM::shaderID, "ShadowMap"), 1);

	shader->unbind();

	debug("initializeShader()");
}

// Make sure this is called after initializeShader()
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

	debug("initializeTexture()");
}

void Window::initializeMatrix(){
	// Ensure MODELVIEW to call get()
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	// Light Projection Matrix
	glLoadIdentity();
	gluPerspective(L::FOVY, L::AR, L::NEARZ, L::FARZ);
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
	gluPerspective(C::FOVY, Window::width / Window::height, C::NEARZ, C::FARZ);
	glGetDoublev(GL_MODELVIEW_MATRIX, C::PM.getPointer());

	// Camera View Matrix
	glLoadIdentity();
	gluLookAt(
		C::position[X], C::position[Y], C::position[Z],
		C::lookAt[X], C::lookAt[Y], C::lookAt[Z],
		0, 1, 0
		);
	glGetDoublev(GL_MODELVIEW_MATRIX, C::VM.getPointer());

	C::IVM = C::VM.inverse();

	glPopMatrix();

	debug("initializeMatrix()");
}

// Updates window
void Window::update(){
	// Rotate and rotate!
	L::position[X] = L::MOVEMENT * cos(glutGet(GLUT_ELAPSED_TIME) / 1000.0);
	L::position[Z] = L::MOVEMENT * sin(glutGet(GLUT_ELAPSED_TIME) / 1000.0);
}

void Window::setTextureMatrix(){

}

///*
void Window::draw(){
	scene->draw(Matrix4d());
}
//*/


// First pass, light POV
void Window::firstPassSM(){

	//Disable color rendering, we only want to write to the Z-Buffer
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	glViewport(0, 0, SM::WIDTH, SM::HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(L::PM.getPointer());

	// TESTING
	//glPushMatrix();
	// Ensure it's MODELVIEW to ensure we can get() later
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		L::position[X], L::position[Y], L::position[Z],
		L::lookAt[X], L::lookAt[Y], L::lookAt[Z],
		0, 1, 0
		);
	glGetDoublev(GL_MODELVIEW_MATRIX, L::VM.getPointer());
	//glPopMatrix();
	// TESTING END

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(L::VM.getPointer());

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);
	glDrawBuffers(0, NULL); 
	glReadBuffer(GL_NONE);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, SM::texture, 0);

	glClear(GL_DEPTH_BUFFER_BIT);



	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glCullFace(GL_FRONT);

	// FIRST RENDER
	draw();

	glCullFace(GL_BACK);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	debug("firstPassSM()");
}

// Second pass, camera POV
void Window::secondPassSM(){

}

void Window::thirdPassSM(){

	//Enabling color write (previously disabled for light POV z-buffer rendering)
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	glViewport(0, 0, Window::width, Window::height);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(C::PM.getPointer());

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(C::VM.getPointer());

	glLightfv(GL_LIGHT0, GL_POSITION, L::position);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glCullFace(GL_BACK);

	static Matrix4d bias(
		0.5, 0.0, 0.0, 0.5,
		0.0, 0.5, 0.0, 0.5,
		0.0, 0.0, 0.5, 0.5,
		0.0, 0.0, 0.0, 1.0
		);

	debug("thirdPassSM() before uniform");

	shader->bind();

	// Get offset
	glUniform1fARB(SM::uniformX, 1.0 / SM::WIDTH);
	glUniform1fARB(SM::uniformY, 1.0 / SM::HEIGHT);

	Matrix4d temp = bias * L::PM * L::VM * C::IVM;

	float test[16];
	for (int i = 0; i < 4; ++i){
		for (int j = 0; j < 4; ++j){
			test[i * 4 + j] = temp.get(i, j);
		}
	}

	debug("thirdPassSM() middle uniform");

	glUniformMatrix4fv(SM::matrix, 1, GL_FALSE, test);

	debug("thirdPassSM() after uniform");

	glActiveTexture(GL_TEXTURE1); 
	glBindTexture(GL_TEXTURE_2D, SM::texture);

	glUniform1i(shading, mode);

	// Enable texture
	glActiveTexture(GL_TEXTURE0);

	// SECOND RENDER
	draw();

	// DISABLE TEXTURE
	glUniform1i(shading, LIGHT);

	/*
	glLoadMatrixf(&ViewMatrix);
	glTranslatef(0.0f, -0.375f, 0.5f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glutSolidTorus(0.125f, 0.25f, 64, 64);
	*/

	glDisable(GL_CULL_FACE);

	/*
	glLoadMatrixf(&ViewMatrix);
	glTranslatef(0.0f, -0.31f, -0.5f);
	glutSolidTeapot(0.25f);
	*/

	// Reset
	glActiveTexture(GL_TEXTURE1); 
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D, 0);

	shader->unbind();

	glDisable(GL_DEPTH_TEST);

	debug("thirdPassSM()");
}

void Window::drawSM(){

	firstPassSM();
	secondPassSM();
	thirdPassSM();
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
	
	// Ensure MODELVIEW 
	glMatrixMode(GL_MODELVIEW);

	//glPushMatrix();
	
	drawSM();
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glPushMatrix();
	glBegin(GL_LINES);

	// LIGHT
	glColor3f(1.0, 1.0, 0.5);
	glVertex3f(L::position[X], 0.0, L::position[Z]);
	glVertex3f(L::position[X], 10000.0, L::position[Z]);
	glVertex3f(L::position[X], 0.0, L::position[Z]);
	glVertex3f(L::position[X], -10000.0, L::position[Z]);

	glColor3f(1.0, 0.5, 0.5);
	glVertex3f(L::position[X], L::position[Y], 0.0);
	glVertex3f(L::position[X], L::position[Y], 10000.0);
	glVertex3f(L::position[X], L::position[Y], 0.0);
	glVertex3f(L::position[X], L::position[Y], -10000.0);

	glColor3f(0.5, 0.5, 1.0);
	glVertex3f(0.0, L::position[Y], L::position[Z]);
	glVertex3f(10000.0, L::position[Y], L::position[Z]);
	glVertex3f(0.0, L::position[Y], L::position[Z]);
	glVertex3f(-10000.0, L::position[Y], L::position[Z]);

	// ORIGIN
	glColor3f(0, 0, 0);
	glVertex3f(0, 0.0, 0);
	glVertex3f(0, 10000.0, 0);
	glVertex3f(0, 0.0, 0);
	glVertex3f(0, -10000.0, 0);

	glColor3f(1, 1, 1);
	glEnd();

	glPopMatrix();
	glDisable(GL_CULL_FACE);
	
	//glPopMatrix();

	glutSwapBuffers();

	debug("displayCallback()");
}

void Window::reshapeCallback(int w, int h){
	Window::width = w;
	Window::height = h;

	glPushMatrix();
	// Ensure it's MODELVIEW so we can use get() later
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluPerspective(C::FOVY, (float)width / height, C::NEARZ, C::FARZ);

	//glGetDoublev(GL_PROJECTION_MATRIX, C::PM.getPointer());
	glGetDoublev(GL_MODELVIEW_MATRIX, C::PM.getPointer());
	glPopMatrix();
}

void Window::debug(const string& comment){

	if (DEBUG){
		GLuint error = glGetError();
		if (error != GL_NO_ERROR){
			cerr << hex << error << " ON " << comment << endl;
			system("PAUSE");
		}
	}
}

void Window::idleCallback(){
	displayCallback();
}

void Window::processKeyboard(unsigned char key, int x, int y){
	switch (key){
	case 27: // ESC
		exit(0);
		break;
	case '1':
		mode = LIGHT;
		break;
	case '2':
		mode = TEXTURE;
		break;
	case '3':
		mode = SHADOW;
		break;
	}
}

void Window::processSpecial(int key, int x, int y){
	
	GLdouble move = 1.0;
	
	switch (key){
	case GLUT_KEY_F1: // Shadow Map debug mode toggle
		mode = LIGHT;
		break;
	case GLUT_KEY_F2: // Toggle shadow
		mode = TEXTURE;
		break;
	case GLUT_KEY_F3:
		mode = SHADOW;
		break;
	case GLUT_KEY_F4:
		update_on = !update_on;
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
}