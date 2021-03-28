#include<GL/glut.h>
#include<map>
#include<string>
#include<tuple>
#include"Particle.h"
#include"Cube.h"
using namespace std;

// Initialize the Eye
map<string, GLfloat> Eye = {
	{"posX", 5.0f}, {"posY", 5.0f}, {"posZ", 5.0f},
	{"lookAtX", 0.0f}, {"lookAtY", 0.0f}, {"lookAtZ", 0.0f},
	{"upX", 0.0f}, {"upY", 1.0f}, {"upZ", 0.0f}
};
bool EyeFollowParticle = false; 

// Light configuration
GLfloat lightSource[] = {5.0f, 5.0f, 5.0f, 1.0f}; //position
GLfloat light[] = {0.0f, 1.0f, 0.0f, 1.0f}; // color

// Create a sphere
Particle ps = Particle(make_tuple(2.5f, 2.5f, 2.5f),
	make_tuple(224, 17, 95), 0.1f, 1);

// Cube's edge length, boundaries of Eye's positions
GLfloat edgeLength = 10.0f;
GLfloat lowerBoundary = 0.1f;
GLfloat upperBoundary = edgeLength - lowerBoundary;

void limitEyePosition() {
	// Set lower boundary for x, y, z
	if (Eye["posX"] < lowerBoundary) Eye["posX"] = lowerBoundary;
	if (Eye["posY"] < lowerBoundary) Eye["posY"] = lowerBoundary;
	if (Eye["posZ"] < lowerBoundary) Eye["posZ"] = lowerBoundary;
	
	// Set upper boundary for x, y, z
	if (Eye["posX"] > upperBoundary) Eye["posX"] = upperBoundary;
	if (Eye["posY"] > upperBoundary) Eye["posY"] = upperBoundary;
	if (Eye["posZ"] > upperBoundary) Eye["posZ"] = upperBoundary;
}

Cube cube = Cube(edgeLength);

void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Light
	glClearDepth(1.0f); // set depth clear
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); // single light source
	glLightfv(GL_LIGHT0, GL_POSITION, lightSource); // init light source
	//glLightfv(GL_LIGHT0, GL_AMBIENT, light);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, light);
	glEnable(GL_COLOR_MATERIAL); // enable material color tracking
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); // set it
	glEnable(GL_BLEND); // enable blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // set it
	
	// Put functions to draw and to move objects here
	cube.draw();
	ps.draw();
	ps.move(lowerBoundary, upperBoundary);

	glutSwapBuffers();
	glLoadIdentity();

	limitEyePosition();

	if (EyeFollowParticle) {
		// This will make the eye follow the sphere
		Eye["lookAtX"] = get<0>(ps.getPosition());
		Eye["lookAtY"] = get<1>(ps.getPosition());
		Eye["lookAtZ"] = get<2>(ps.getPosition());
	}
	
	gluLookAt(Eye["posX"], Eye["posY"], Eye["posZ"],
		Eye["lookAtX"], Eye["lookAtY"], Eye["lookAtZ"],
		Eye["upX"], Eye["upY"], Eye["upZ"]);

}

void reshapeScene(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, width, height);
	gluPerspective(60, 1.0f, 1.0, 100);
	glMatrixMode(GL_MODELVIEW);
}

// This func is a para of glutKeyBoardFunc()
void keyboard(unsigned char key, int x_mouse_pos, int y_mouse_pos) {
	switch (key)
	{
	case 27: // ESC key
		exit(0);
		break;
	case 102: // f key
		EyeFollowParticle = !EyeFollowParticle;
		if (!EyeFollowParticle) {
			// This will make the eye look at 0, 0, 0 corner
			Eye["lookAtX"] = 0.0f;
			Eye["lookAtY"] = 0.0f;
			Eye["lookAtZ"] = 0.0f;
		}
		break;
	default:
		break;
	}
}


int main(int argc, char **argv) {
	glutInit(&argc, argv);

	// Set color mode
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	
	// Set window width, height
	glutInitWindowSize(640, 640);

	// Set window position
	glutInitWindowPosition(0, 0);

	// Set Set window name
	glutCreateWindow("Particle Simulation");

	// Pass a function to draw
	glutDisplayFunc(renderScene);

	// Keep that function alive
	glutIdleFunc(renderScene);

	// Pass a function to handle the change of window
	glutReshapeFunc(reshapeScene);

	// Pass functions to handle keyboard events
	glutKeyboardFunc(keyboard);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	// Load texture
	ps.loadCustomBmp("metal.bmp");

	glutMainLoop();
	return 0;
}