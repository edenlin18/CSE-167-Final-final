#include "main.h"

// GL Settings goes here
void setting(){

}

// Driver
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	
	//glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutGameModeString("1920¡Á1080:32@75");
	glutEnterGameMode();
	//glutInitWindowPosition(1000,100);
	//glutInitWindowSize(Window::width, Window::height);
	//glutCreateWindow("Fly!");

	setting();

	// Initialize window
	Window::initialize();

	glutFullScreen();
	
	// Callbacks
	glutReshapeFunc(Window::reshapeCallback);
	glutDisplayFunc(Window::displayCallback);
	glutIdleFunc(Window::idleCallback);
	
	glutKeyboardFunc(Window::processKeyboard);
	glutSpecialFunc(Window::processSpecial);
	
	glutMainLoop();
}