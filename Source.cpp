#include "iostream"
#include "fstream"
#include "string"
#include <sstream>
#include <stdlib.h>
#include "gl/glut.h"


using namespace std;

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ifstream outfile("testpoints.txt");
	if (!outfile){
		cout << "Unable to open otfile";
		exit(1); // terminate with error
	}
	char line[50];
	char x[10], y[10], z[10];

	glBegin(GL_POINTS);
	while (outfile.getline(line, sizeof(line)))
	{
		stringstream word(line);
		word >> x;
		word >> y;
		word >> z;
		glVertex3f(atoi(x), atoi(y), atoi(z));
		//cout << atoi(x)  << " " << atoi(y)  << " " << atoi(z)  << endl;
	}
	glEnd();
	outfile.clear();
	outfile.close();
	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, 2 * (GLsizei)w, 2 * (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 2 * (GLdouble)w, 0.0, 2 * (GLdouble)h);
	gluLookAt(-20.0, 0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500,500);
	glutCreateWindow("dis");
	init();
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}