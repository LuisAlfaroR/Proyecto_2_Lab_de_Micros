// A Simple OpenGL Project
// Author: Michael Hall
//
// This C++ code and project are provided "as is" without warranty of any kind.
//
// Copyright 2010 XoaX - For personal use only, not for distribution
#include <GL/glut.h>
#include <string>
#include <stdio.h>
#include <iostream>

using namespace std;
float x=0.0;
static float posx1=0.25;
static float posx2=0.40;

void Draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glColor3f(0.0, 0.2, 0.0);
	glBegin(GL_QUAD_STRIP);
		glVertex3f(posx1, 0.25, 0.0);
		glVertex3f(posx1, 0.30, 0.0);
		glVertex3f(posx2, 0.25, 0.0);
		glVertex3f(posx2, 0.30, 0.0);
	glEnd();
	glPopMatrix();
	glutSwapBuffers();
	glFlush();
}

void Initialize() {
	glClearColor(0.10, 0.25, 0.3, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void moverderecha(void) {
	if(posx1<=0.85){
		x=x+0.000001;
		posx1=posx1+x;
		posx2=posx2+x;
		}
	glutPostRedisplay();
}

void moverizquier(void) {
	if(posx1>=0){
		x=x+0.000001;
		posx1=posx1-x;
		posx2=posx2-x;
		}
	glutPostRedisplay();
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case 120:
		glutIdleFunc(moverderecha);
        break;
      case 122:
		glutIdleFunc(moverizquier);
		break;
   }
}


int main(int iArgc, char** cppArgv) {
	glutInit(&iArgc, cppArgv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 480);
	glutInitWindowPosition(00, 00);
	glutCreateWindow("XoaX.net");
	Initialize();
	glutDisplayFunc(Draw);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
