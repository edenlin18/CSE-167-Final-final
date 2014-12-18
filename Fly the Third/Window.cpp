#include "Window.h"

#define ROTSCALE 50.0;
#define ZOOMSCALE 2.0;
#define DELTA 0.1
#define ANGLE 1.0
#define TIME_INTERVAL 0.001


GLfloat Window::width = 640.0;
GLfloat Window::height = 480.0;

GLuint Window::fboID;

Shader* Window::shader;

Scene* Window::scene;

MatrixTransform* skyboxPos;
SkyBox* skybox;

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





enum {
	OFF = 0, ON = 1
};

// Texture
namespace T{
	GLuint texturing;
	int mode = OFF;
}

// Shadow Map
namespace SM{
	const int RATIO = 2;

	const GLfloat WIDTH = 3000;
	const GLfloat HEIGHT = 3000;
	//const GLfloat HEIGHT = 2048.0;

	GLhandleARB shaderID;
	GLuint uniform;
	GLuint uniformX;
	GLuint uniformY;

	GLuint shadowing;
	int mode = OFF;

	// Depth texture
	GLuint texture;

	GLuint matrix;
}

// Light
namespace L{
	float position[3] = { -25, 50, 25 };
	float lookAt[3] = { 0, 0, 0 };
	const float MOVEMENT = 30.0;

	//gluPerspective(35.0f, 1.0f, 4.0f, 16.0f);
	//static vec3 LightPosition = vec3(0.0f, 2.5f, 5.0f);

	const GLdouble FOVY = 75;
	const GLdouble AR = 1.0;
	const GLdouble NEARZ = 1.0;
	const GLdouble FARZ = 1000.0;
	
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
	float position[3] = { 5, 20, 20 };
	//float lookAt[3] = { 2, 0, -10 };
	float lookAt[3] = { 0, 0, 0 };

	//Camera.Look(vec3(1.75f, 1.75f, 5.0f), vec3(0.0f, 0.0f, 0.0f));

	const GLdouble FOVY = 60;
	const GLdouble NEARZ = 0.001;
	const GLdouble FARZ = 1000.0;

	//perspective(45.0f, (float)Width / (float)Height, 0.125f, 512.0f);

	// View & Project Matrix
	Matrix4d VM;
	Matrix4d IVM;
	Matrix4d PM;
}

// MERGE DOWN

struct MovingCamera {
	Camera* cam;
	double delta;
	double t;
	PiecewiseBezierCurve * pbc;
	MovingCamera(Vector3d lookat, Vector3d up, double _delta, PiecewiseBezierCurve * _pbc) {
		delta = _delta;
		t = 0.0;
		pbc = _pbc;
		cam = new Camera(pbc->getCp(0), lookat, up);
	}
	void move() {
		t += delta;
		if (t > 1.0)
			t = 0.0;
		cam->move(pbc->compute(t));
	}
	void lookat(Vector3d lookat) {
		cam->setLookAt(lookat);
	}
};

struct PlaneCamera {
	Camera* cam;
	double delta;
	double tc;
	double tp;
	PiecewiseBezierCurve * pbc;
	Airplane * plane;
	PlaneCamera(Vector3d up, double _delta, double interval, PiecewiseBezierCurve * _pbc) {
		delta = _delta;
		tc = 0.0;
		tp = tc + interval;
		pbc = _pbc;
		Vector3d v = pbc->compute(tp);
		plane = new Airplane(v, pbc->compute(tp + delta));
		cam = new Camera(pbc->getCp(0), plane->getPosition(), up);
	}
	void move() {
		tc += delta;
		tp += delta;
		if (tc > 1)
			tc = 0.0;
		if (tp > 1)
			tp = 0.0;
		cam->move(pbc->compute(tc));
		plane->move(pbc->compute(tp));
		cam->setLookAt(plane->getPosition());
	}
};

//trackball variables
control::MOVEMENT movement = control::NONE;
Vector3d lastPoint;
Matrix4d rotation;
Matrix4d scaling;
MatrixTransform* Window::scaling_mt = new MatrixTransform(scaling);
MatrixTransform* Window::rotate_mt = new MatrixTransform(rotation);
MatrixTransform* root;
City* c;
Airplane* plane;

Vector3d oldPos;
Vector3d newPos;
double t = 0.06;
double tc = 0.0;
MatrixTransform * tt;
MatrixTransform * mt;
Turtle3D * turtle;
PiecewiseBezierCurve * pbc;

PlaneCamera* pc;
MovingCamera ** mc;
int choice = 4;

bool selected = false;
int g = 1;

MatrixTransform* tree;

void Window::init() {
	///*
	turtle = new Turtle3D("ABCDE", "ABCDE", 13.5, 10, 0.9);
	turtle->addRule('A', "FFFFFFF[-\AE][++&AE]F[+&+AE]FFF", 0.5);
	turtle->addRule('A', "FFFFFFF[+++//AE]F[+&+AE]FFF", 0.5);
	//turtle->addRule('B', "^^^FF", 0.33);
	turtle->addRule('B', "FF[&FF]FF", 0.5);
	turtle->addRule('B', "F[^FF]FF", 0.5);
	turtle->addRule('C', "F[/+CE]F[^AE]F", 0.5);
	turtle->addRule('C', "F[\-CE]F[&AE]F", 0.5);
	turtle->addRule('D', "FF[&F]F",1.0);
	turtle->addRule('E', "[-/F+L-L][^-F^L&L][\FF&L^L--L][+F/L&L+L][&F\L&&L][+FFL&L^L--L]FFL",1.0);

	root = new MatrixTransform(Matrix4d());
	mt = turtle->generate(g);
	Matrix4d t;
	t.makeTranslate(0, -1, 0);
	tt = new MatrixTransform(t);
	root->addChild(rotate_mt);
	rotate_mt->addChild(scaling_mt);
	scaling_mt->addChild(tt);
	//tt->addChild(mt);
	//*/

	///*

	



	Vector3d * v = new Vector3d[5];
	v[0] = Vector3d(0, fRand(10, 20), 0);
	v[1] = Vector3d(20, fRand(10, 20), 0);
	v[2] = Vector3d(20, fRand(10, 20), -10);
	v[3] = Vector3d(0, fRand(10, 20), -10);
	//v[4] = Vector3d(0, 30, -8);
	pbc = new PiecewiseBezierCurve(4, v, 10000, true);
	//cam.move( pbc->getCp(0));
	//oldPos = pbc->getCp(0);



	// test
	srand(time(NULL));
	c = new City(mt);
	c->init();
	root->addChild(c->getRoot());
	//plane = new Airplane(pbc->getCp(0),pbc->getCp(1) - pbc->getCp(0));
	pc = new PlaneCamera(Vector3d(0, 1, 0), 0.0009, 0.1, pbc);


	root->addChild(pc->plane->getRoot());
	//root->addChild(skybox);

	Vector3d * v1 = new Vector3d[4];
	v1[0] = Vector3d(0, fRand(2, 6), 0);
	v1[1] = Vector3d(20, fRand(2, 6), 0);
	v1[2] = Vector3d(20, fRand(2, 6), -10);
	v1[3] = Vector3d(0, fRand(2, 6), -10);

	mc = new MovingCamera*[4];
	mc[0] = new MovingCamera(Vector3d(0, 1, 0), Vector3d(1, 0, 0), 0.00005, new PiecewiseBezierCurve(4, v1, 1000, true));

	Vector3d * v2 = new Vector3d[4];
	v2[0] = Vector3d(-5, fRand(3, 8), 10);
	v2[1] = Vector3d(2.5, fRand(3, 8), 0);
	v2[2] = Vector3d(20, fRand(3, 8), -10);
	v2[3] = Vector3d(0, fRand(3, 8), -10);
	mc[1] = new MovingCamera(Vector3d(0, 1, 1), Vector3d(1, 1, 0), 0.0005, new PiecewiseBezierCurve(4, v2, 1000, true));

	mc[2] = new MovingCamera(Vector3d(0, 1, 1), Vector3d(1, 1, 0), 0.0003, new PiecewiseBezierCurve(4, v1, 1000, true));

	Vector3d * v3 = new Vector3d[4];
	v3[0] = Vector3d(5, fRand(2, 6), 5);
	v3[1] = Vector3d(-5, fRand(2, 6), 5);
	v3[2] = Vector3d(-5, fRand(2, 6), -5);
	v3[3] = Vector3d(5, fRand(2, 6), -5);

	mc[3] = new MovingCamera(Vector3d(0, 0, 0), Vector3d(0, 1, 0), 0.0003, new PiecewiseBezierCurve(4, v3, 1000, true));
}

void Window::initialize(){
	
	initializeFBO();
	initializeShader();
	initializeTexture();
	initializeMatrix();


	// Skybox
	Matrix4d temp;
	temp.makeScale(50.0, 1.0, 50.0);
	skybox = new SkyBox();
	skybox->init();

	// SCENE
	// scene = new Scene();
	// scene->initialize();
	init();


	
	// LIGHTING
	glLightfv(GL_LIGHT0, GL_POSITION, L::position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, L::ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, L::diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, L::specular);

	


	firstPassSM();

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

	//shading = glGetUniformLocationARB(SM::shaderID, "Shading");

	SM::shadowing = glGetUniformLocationARB(SM::shaderID, "shadowing");
	T::texturing = glGetUniformLocationARB(SM::shaderID, "texturing");

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
void Window::draw(Matrix4d m){
	root->draw(m);	

	//scene->draw(Matrix4d());
	/*
	if (choice == 3) {
		root->draw(m);
	}
	else {
		root->draw(m);
	}
	//pc->pbc->render(m);
	*/
	//pc->plane->getRoot()->draw(pc->cam->getMatrix());

	debug("draw()");
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
	draw(L::VM);

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
	//glLoadMatrixd(C::VM.getPointer());
	///*
	if (choice == 4) {
		glLoadMatrixd(pc->cam->getMatrix().getPointer());
	}
	else {
		glLoadMatrixd(mc[choice]->cam->getMatrix().getPointer());
	}
	//*/
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

	///*
	Matrix4d temp;
	if (choice == 4) {
		temp = bias * L::PM * L::VM * pc->cam->getMatrix().inverse();
	}
	else {
		temp = bias * L::PM * L::VM * mc[choice]->cam->getMatrix().inverse();
	}
	//*/
	//Matrix4d temp = bias * L::PM * L::VM * C::IVM;

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

	// ENABLE SHADOWS AND TEXTURES
	glUniform1i(SM::shadowing, SM::mode);
	glUniform1i(T::texturing, T::mode);

	// Enable texture
	glActiveTexture(GL_TEXTURE0);

	// SECOND RENDER
	if (choice == 4) {
		draw(pc->cam->getMatrix());
	}
	else {
		draw(mc[choice]->cam->getMatrix());
	}

	// DISABLE TEXTURE
	//glUniform1i(SM::shadowing, OFF);
	//glUniform1i(T::texturing, T::mode);

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

	if (choice == 4) {
		drawP(pc->cam->getMatrix());
	}
	else {
		drawP(mc[choice]->cam->getMatrix());
	}

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

}

void Window::drawP(Matrix4d m) {
	//pc->plane->getRoot()->draw(pc->cam->getMatrix());
	//float size = 100;
	//Matrix4d temp;
	//temp.makeScale(size, size, size);

	/*if (choice == 3) {
		skybox->draw(pc->cam->getMatrix());
	}
	else {
		skybox->draw(mc[choice]->cam->getMatrix());
	}*/

	skybox->draw(m);

}

void Window::displayCallback() {

	// Ensure MODELVIEW 
	glMatrixMode(GL_MODELVIEW);

	//glPushMatrix();

	drawSM();

	/*
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glPushMatrix();
	glLoadIdentity();

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
	*/

	//glPopMatrix();


	// skybox
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	Matrix4d temp;
	temp.makeScale(1, 1, 1);
	// temp.makeTranslate(-C::position[X], -C::position[Y], -C::position[Z]);
	glLoadMatrixd((temp).getPointer());
	//glLoadIdentity();
	//skybox->render();

	/*glColor3d(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3d(-1, 1, -1);
	glVertex3d(-1, -1, -1);
	glVertex3d(1, -1, -1);
	glVertex3d(1, 1, -1);
	glEnd();*/
	glPopMatrix();

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

double Window::fRand(double fMin, double fMax) {
	double f = (double) rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
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
	pc->move();
	for (int i = 0; i < 4; i++)
		mc[i]->move();
	mc[0]->lookat(pc->plane->getPosition());
	mc[1]->lookat(mc[0]->pbc->compute(mc[0]->t));
	mc[2]->lookat(mc[1]->pbc->compute(mc[1]->t));
	mc[3]->lookat(Vector3d(0, 0, 0));
}

void Window::processKeyboard(unsigned char key, int x, int y){
	switch (key){
	case 27: // ESC
		exit(0);
		break;
	case 'r':
		root->removeChild(c->getRoot());
		c = new City(mt);
		c->init();
		root->addChild(c->getRoot());
		break;
	case 't':
		if (g > 1) {
			g = 1;
			mt = turtle->generate(g);
		}
		break;
	case 'g':
		if (g >= 5) {
			break;
		}
		g++;
		mt = turtle->generate(g);
		c->treeGrow(mt);

		cerr << "SIZE " << g << endl;
		break;
	case 'f':
		if (g == 0)
			return;
		g--;
		tt->removeChild(mt);
		delete mt;
		mt = turtle->generate(g);
		tt->addChild(mt);
		break;
	case 'd':
		selected = false;
		pbc->deSelect();
		break;
	case 'x':
		if (selected) {
			int index = pbc->getSelected();
			Vector3d v = pbc->getCp(index);
			v.set(0, v[0] + DELTA);
			pbc->setCp(index, v);
			cerr << "sdf";
		}
		break;
	case 'X':
		if (selected) {
			int index = pbc->getSelected();
			Vector3d v = pbc->getCp(index);
			v.set(0, v[0] - DELTA);
			pbc->setCp(index, v);
		}
		break;
	case 'y':
		if (selected) {
			int index = pbc->getSelected();
			Vector3d v = pbc->getCp(index);
			v.set(1, v[1] + DELTA);
			pbc->setCp(index, v);
		}
		break;
	case 'Y':
		if (selected) {
			int index = pbc->getSelected();
			Vector3d v = pbc->getCp(index);
			v.set(1, v[1] - DELTA);
			pbc->setCp(index, v);
		}
		break;
	case 'z':
		if (selected) {
			int index = pbc->getSelected();
			Vector3d v = pbc->getCp(index);
			v.set(2, v[2] + DELTA);
			pbc->setCp(index, v);
		}
		break;
	case 'Z':
		if (selected) {
			int index = pbc->getSelected();
			Vector3d v = pbc->getCp(index);
			v.set(2, v[2] - DELTA);
			pbc->setCp(index, v);
		}
		break;
	case 'c':{
		/*
		t += TIME_INTERVAL;
		if (t > 1)
		t = 0.0;
		tc += TIME_INTERVAL;

		// std::cerr << "tc: " << tc << " t: " << t << std::endl;
		cam.move(pbc->compute(tc));
		Vector3d l = pbc->compute(t);
		plane->move(l);
		cam.setLookAt(l);
		oldPos = newPos;
		*/
		pc->move();
	}
		break;
	case 'a':
		choice++;
		choice = choice % 5;
		break;
	case '1':
		choice = 1;
		break;
	case '2':
		choice = 2;
		break;
	case '3':
		choice = 3;
		break;
	case '4': // Plane Camera
		choice = 4;
	}
}

void Window::processSpecial(int key, int x, int y){
	
	GLdouble move = 1.0;
	
	switch (key){
	case GLUT_KEY_F1: // Shadow Map debug mode toggle
		SM::mode = OFF;
		T::mode = OFF;
		break;
	case GLUT_KEY_F2: // Toggle shadow
		if (T::mode) {
			T::mode = OFF;
		}
		else {
			T::mode = ON;
		}
		break;
	case GLUT_KEY_F3:
		if (SM::mode) {
			SM::mode = OFF;
		}
		else {
			SM::mode = ON;
		}
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