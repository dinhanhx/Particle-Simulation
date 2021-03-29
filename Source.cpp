#include<GL/glut.h>
#include<map>
#include<string>
#include<tuple>
#include"Particle.h"
#include"Cube.h"
#include"WaterParticle.h"
using namespace std;

// Initialize the Eye
map<string, GLfloat> Eye = {
	{"posX", 5.0f}, {"posY", 5.0f}, {"posZ", 5.0f},
	{"lookAtX", 0.0f}, {"lookAtY", 0.0f}, {"lookAtZ", 0.0f},
	{"upX", 0.0f}, {"upY", 1.0f}, {"upZ", 0.0f}
};
bool EyeFollowParticle = true; 

// Light configuration
GLfloat lightSource[] = {5.0f, 5.0f, 5.0f, 1.0f}; //position
GLfloat light[] = {0.0f, 1.0f, 0.0f, 1.0f}; // color

// Create a water particle
WaterParticle wp0 = WaterParticle(make_tuple(2.5f, 2.5f, 2.5f), 0);
WaterParticle wp1 = WaterParticle(make_tuple(3.5f, 3.5f, 3.5f), 1);
WaterParticle wp2 = WaterParticle(make_tuple(4.5f, 4.5f, 4.5f), 2);

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

// Eye's control and config
float vertical = 0.0f;
float horizontal = 0.0f;
float deltaHorizontal = 0.0f;
float deltaVertical = 0.0f;
float lx = cos(horizontal) * cos(vertical);
float ly = sin(vertical);
float lz = sin(horizontal) * cos(vertical);

void computeHorizontal(float deltaAngle) {
	horizontal += deltaAngle;
	lx = cos(horizontal) * cos(vertical);
	lz = sin(horizontal) * cos(vertical);
}

void computeVertical(float deltaAngle) {
	vertical += deltaAngle;
	ly = sin(vertical);
}

// This func is a para of glutDisplayFunc, glutIdleFunc
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
	wp0.draw();
	wp0.move(lowerBoundary, upperBoundary);

	wp1.draw();
	wp1.move(lowerBoundary, upperBoundary);

	wp2.draw();
	wp2.move(lowerBoundary, upperBoundary);

	glutSwapBuffers();
	glLoadIdentity();

	limitEyePosition();

	if (EyeFollowParticle) {
		// This will make the eye follow the sphere
		Eye["lookAtX"] = get<0>(wp0.getPosition());
		Eye["lookAtY"] = get<1>(wp0.getPosition());
		Eye["lookAtZ"] = get<2>(wp0.getPosition());

		gluLookAt(Eye["posX"], Eye["posY"], Eye["posZ"],
			Eye["lookAtX"], Eye["lookAtY"], Eye["lookAtZ"],
			Eye["upX"], Eye["upY"], Eye["upZ"]);
	}
	else {
		if (deltaHorizontal) {
			computeHorizontal(deltaHorizontal);
		}
		if (deltaVertical) {
			computeVertical(deltaVertical);
		}

		gluLookAt(Eye["posX"], Eye["posY"], Eye["posZ"],
			Eye["posX"] + lx, Eye["posY"] + ly, Eye["posZ"] + lz,
			Eye["upX"], Eye["upY"], Eye["upZ"]);
	}
	
	

}

// This func is a para of glutReshapeFunc()
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
			gluLookAt(Eye["posX"], Eye["posY"], Eye["posZ"],
				0.0f, 0.0f, 0.0f,
				Eye["upX"], Eye["upY"], Eye["upZ"]);
		}
		break;
	default:
		break;
	}
}

// This func is a para of glutKeyboardSpecialFunc()
void pressKey(int key, int mouseX, int mouseY) {
	switch (key)
	{
	case GLUT_KEY_UP:
		deltaVertical = 0.001f;
		break;
	case GLUT_KEY_DOWN:
		deltaVertical = -0.001f;
		break;
	case GLUT_KEY_LEFT:
		deltaHorizontal = -0.001f;
		break;
	case GLUT_KEY_RIGHT:
		deltaHorizontal = 0.001f;
		break;
	default:
		break;
	}
}

// This func is a para of glutKeyboardSpecialUpFunc()
void releaseKey(int key, int mouseX, int mouseY) {
	switch (key)
	{
	case GLUT_KEY_UP:
		deltaVertical = 0.0f;
		break;
	case GLUT_KEY_DOWN:
		deltaVertical = 0.0f;
		break;
	case GLUT_KEY_LEFT:
		deltaHorizontal = 0.0f;
		break;
	case GLUT_KEY_RIGHT:
		deltaHorizontal = 0.0f;
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
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glutMainLoop();
	return 0;
}