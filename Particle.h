#pragma once
#pragma warning(disable : 4996)
#include <tuple>
#include <GL/glut.h>
#include <time.h>
using namespace std;

class Particle
{
private:
	tuple <float, float, float> position; //x, y, z position
	tuple <float, float, float> velocity; //x, y, z velocity
	tuple <GLubyte, GLubyte, GLubyte> color; // rgb color code
	float radius;
	int id;

	// Texture
	GLUquadricObj* quadricObj = gluNewQuadric();
	GLuint textureID;
public:
	Particle(tuple <float, float, float> position, 
			tuple <unsigned short, unsigned short, unsigned short> color, 
			float radius, int id) {
		this->position = position;
		this->color = color;
		this->radius = radius;
		this->id = id;

		// Assign random velocity for x, y, z
		srand(static_cast <unsigned> (time(0)));
		float LO = 0.000f;
		float HI = 0.005f;
		float x_vec = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
		float y_vec = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
		float z_vec = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
		velocity = make_tuple(x_vec, y_vec, z_vec);
	}

	Particle () {}

	// Get functions
	tuple <float, float, float> getPosition() { return position; }
	tuple <unsigned short, unsigned short, unsigned short> getColor() { return color; }
	float getRadius() { return radius; }
	int getId() { return id; }

	// Set functions
	void setPosition(tuple <float, float, float> position) { this->position = position; }
	void setVelocity(tuple <float, float, float> velocity) { this->velocity = velocity; }

	// Draw a solid sphere
	void draw()
	{
		glTranslatef(0.0f, 0.0f, 0.0f);
		glPushMatrix();

		if (textureID == 0) glColor3ub(get<0>(color),
										get<1>(color),
										get<2>(color));

		glTranslatef(get<0>(position),
			get<1>(position),
			get<2>(position));

		gluQuadricDrawStyle(quadricObj, GLU_FILL);
		glBindTexture(GL_TEXTURE_2D, textureID);
		gluQuadricTexture(quadricObj, GL_TRUE);
		gluQuadricNormals(quadricObj, GLU_SMOOTH);
		gluSphere(quadricObj, radius, 16, 32);

		glPopMatrix();
	}

	// Move the sphere under circumstances
	void move(GLfloat lowerBoundary, GLfloat upperBoundary) {
		// bouncing x
		if (get<0>(position) > upperBoundary || get<0>(position) < lowerBoundary) get<0>(velocity) = -1.0f * get<0>(velocity);
		// bouncing y
		if (get<1>(position) > upperBoundary || get<1>(position) < lowerBoundary) get<1>(velocity) = -1.0f * get<1>(velocity);
		// bouncing z
		if (get<2>(position) > upperBoundary || get<2>(position) < lowerBoundary) get<2>(velocity) = -1.0f * get<2>(velocity);

		// move x
		get<0>(position) = get<0>(position) + get<0>(velocity);

		// move y
		get<1>(position) = get<1>(position) + get<1>(velocity);

		// move z
		get<2>(position) = get<2>(position) + get<2>(velocity);
	}

	void loadCustomBmp(const char* imagepath) {
		printf("Reading image %s\n", imagepath);

		// Data read from the header of the BMP file
		unsigned char header[54];
		unsigned int dataPos;
		unsigned int imageSize;
		unsigned int width, height;
		// Actual RGB data
		unsigned char* data;

		// Open the file
		FILE* file = fopen(imagepath, "rb");
		if (!file) {
			printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath);
			getchar();
			textureID = 0;
		}

		// Read the header, i.e. the 54 first bytes

		// If less than 54 bytes are read, problem
		if (fread(header, 1, 54, file) != 54) {
			printf("Not a correct BMP file\n");
			fclose(file);
			textureID = 0;
		}
		// A BMP files always begins with "BM"
		if (header[0] != 'B' || header[1] != 'M') {
			printf("Not a correct BMP file\n");
			fclose(file);
			textureID = 0;
		}
		// Make sure this is a 24bpp file
		if (*(int*)&(header[0x1E]) != 0) { printf("Not a correct BMP file\n");    fclose(file); textureID = 0; }
		if (*(int*)&(header[0x1C]) != 24) { printf("Not a correct BMP file\n");    fclose(file); textureID = 0; }

		// Read the information about the image
		dataPos = *(int*)&(header[0x0A]);
		imageSize = *(int*)&(header[0x22]);
		width = *(int*)&(header[0x12]);
		height = *(int*)&(header[0x16]);

		// Some BMP files are misformatted, guess missing information
		if (imageSize == 0)    imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component
		if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

											 // Create a buffer
		data = new unsigned char[imageSize];

		// Read the actual data from the file into the buffer
		fread(data, 1, imageSize, file);

		// Everything is in memory now, the file can be closed.
		fclose(file);

		// Create one OpenGL texture
		glGenTextures(1, &textureID);

		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Give the image to OpenGL
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		// OpenGL has now copied the data. Free our own version
		delete[] data;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Return the ID of the texture we just created
	}
};

