#include "Scene.h"

Scene::Scene(){
}

Scene::~Scene(){
}

void Scene::initialize(){
	turtle = new Turtle3D("ABC-D+E", "ABCDE", 20, 20, 0.8);
	turtle->addRule('A', "FFFFFFF[-\AE]F[+&+AE]FFF", 1.0);
	turtle->addRule('B', "^^^FF", 0.3);
	turtle->addRule('B', "FF&&&FF", 0.3);
	turtle->addRule('B', "+F\\FF", 0.4);
	turtle->addRule('C', "F[/+CE]F[^AE]F", 1.0);
	turtle->addRule('D', "FF&F", 1.0);
	turtle->addRule('E', "[-/F][^-FL][\FFL][+F][&FL][+FFL]FFL", 1.0);
	mt = turtle->generate(5);

	city = new City(mt);
	root = new MatrixTransform(Matrix4d());

	root->addChild(city->getRoot());

	debug("initialize()");
}

void Scene::update(){

	delete city;
	delete root;

	city = new City(mt);
	root = new MatrixTransform(Matrix4d());

	root->addChild(city->getRoot());

	debug("update()");
}

void Scene::draw(Matrix4d C){

	root->draw(Matrix4d());

	//drawDefault();

	debug("draw()");
}

void Scene::drawDefault(){	
	
	// GROUND
	glBegin(GL_QUADS);
	glColor3f(0.7, 1, 0.7);
	glNormal3f(0, 1, 0);
	glVertex3f(1000, 0, 1000);
	glVertex3f(1000, 0, -1000);
	glVertex3f(-1000, 0, -1000);
	glVertex3f(-1000, 0, 1000);
	glEnd();
	

	GLdouble size = 10.0;

	glPushMatrix();
	glColor3f(0.5, 0.5, 1.0);
	//glTranslatef(x, y, z);
	glTranslatef(0, size / 2.0, 0);
	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	glPushMatrix();
	//glTranslatef(x, y, z);


	glTranslatef(0, size / 2.0, 0);
	// glutSolidSphere(size, 20, 20);

	glutSolidCube(size);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Scene::debug(const string& comment) {

	if (DEBUG) {
		GLuint error = glGetError();
		if (error != GL_NO_ERROR) {
			cerr << hex << error << " AT Scene " << comment << endl;
			system("PAUSE");
		}
	}

}

/*
void Scene::initialize(){

	for (int i = 0; i < Building::MAX; ++i){

		srand(time(NULL) + i);

		//rand() % 18 + (-9);
		float x = rand() % 28 + (-19);
		float z = rand() % 28 + (-19);

		building(x, z, object[i]);
		//cerr << x << " " << z << endl;
	}
}

void Scene::update(){

}

/*
void Scene::draw(){

	glPushMatrix();
	//glTranslatef(x, y, z);

	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	glPushMatrix();
	//glTranslatef(x, y, z);

	glBegin(GL_QUADS);
	glColor3f(0.7, 1, 0.7);
	glNormal3f(0, 1, 0);
	glVertex3f(1000, 0, 1000);
	glVertex3f(1000, 0, -1000);
	glVertex3f(-1000, 0, -1000);
	glVertex3f(-1000, 0, 1000);
	glEnd();

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	// Object

	glPushMatrix();
	glColor3f(0.5, 0.5, 1.0);
	//glTranslatef(x, y, z);
	glTranslatef(0, 0.5, 0);
	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	glPushMatrix();
	//glTranslatef(x, y, z);


	glTranslatef(0, 0.5, 0);
	glutSolidCube(1.0);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glBegin(GL_QUADS);
	glColor3f(0.7, 1, 0.7);
	glNormal3f(0, 1, 0);
	glVertex3f(1000, 0, 1000);
	glVertex3f(1000, 0, -1000);
	glVertex3f(-1000, 0, -1000);
	glVertex3f(-1000, 0, 1000);
	glEnd();

	glPushMatrix();
	glColor3f(0.5, 0.5, 1.0);
	glScalef(1, 5, 1);
	//glTranslatef(x, y, z);
	glTranslatef(0, 0.5, 0);
	//glScalef(1, 5, 1);
	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	glPushMatrix();
	glScalef(1, 5, 1);
	glTranslatef(0, 0.5, 0);
	//glScalef(1, 5, 1);
	glutSolidCube(1);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glColor3f(1, 0.8, 0.8);
	glPushMatrix();

	glTranslatef(5, 0.5, 5);
	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	glPushMatrix();
	glTranslatef(5, 0.5, 5);
	glutSolidSphere(1, 20, 20);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5, 0.5, 1.0);
	glScalef(1, 5, 1);
	//glTranslatef(x, y, z);
	glTranslatef(0, 0.5, 0);


	//glScalef(1, 5, 1);
	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	glPushMatrix();
	glScalef(1, 5, 1);
	glTranslatef(0, 0.5, 0);
	//glScalef(1, 5, 1);
	glutSolidCube(1);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
	
}

void Scene::draw(){

	// Ground
	glBegin(GL_QUADS);
	glColor3f(0.7, 1, 0.7);
	glNormal3f(0, 1, 0);
	glVertex3f(1000, 0, 1000);
	glVertex3f(1000, 0, -1000);
	glVertex3f(-1000, 0, -1000);
	glVertex3f(-1000, 0, 1000);
	glEnd();

	for (int i = 0; i < Building::MAX; ++i){

		Matrix4 temp;

		glColor3f(object[i].RGB[0], object[i].RGB[1], object[i].RGB[2]);

		glPushMatrix();
		glGetDoublev(GL_MODELVIEW_MATRIX, temp);
		glLoadMatrixd(temp * object[i].matrix);

		glMatrixMode(GL_TEXTURE);
		glActiveTextureARB(GL_TEXTURE7);
		glPushMatrix();

		glGetDoublev(GL_TEXTURE_MATRIX, temp);
		glLoadMatrixd(temp * object[i].matrix);
		glutSolidCube(1.0);

		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

	}
}

void Scene::building(const GLfloat& x, const GLfloat& z, Building& building){

	srand(abs(x) + abs(z));

	float height = rand() % 10 + 3;
	float r = rand() % 10 + 1;
	float g = rand() % 10 + 1;
	float b = rand() % 10 + 1;

	building.RGB[0] = r / 10.0;
	building.RGB[1] = g / 10.0;
	building.RGB[2] = b / 10.0;

	Matrix4 S;
	S.makeScale(1, height, 1);

	Matrix4 T;
	T.makeTranslate(x, 0.5, z);

	building.matrix = S * T;
}
*/