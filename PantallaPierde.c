#include <X11/Xlib.h>
#include <GL/glx.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <SOIL/SOIL.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>


static GLuint texGameover, texFondo;
static GLint ancho = 100, alto = 100, anchoImagenX = 338, altoImagenY = 137;
float posx = 600;
float posy = 550;

float r = 0.78540;
float theta = 0.78540;
float r0 = 0;
float k = 7;
float deltaTheta = 0.08727;


void init(void){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

texGameover = SOIL_load_OGL_texture(
		"gameoficial.PNG",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);
	
	glBindTexture(GL_TEXTURE_2D, texGameover);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	

texFondo = SOIL_load_OGL_texture(
		"GameOvFondo2.PNG",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);
	
	glBindTexture(GL_TEXTURE_2D, texFondo);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
}
	
void display(void){
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glBindTexture(GL_TEXTURE_2D, texGameover);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(posx, posy, 0.1);
		glTexCoord2f(0.0, 1.0); glVertex3f(posx, posy + altoImagenY, 0.1);
		glTexCoord2f(1.0, 1.0); glVertex3f(posx + anchoImagenX, posy + altoImagenY, 0.1);
		glTexCoord2f(1.0, 0.0); glVertex3f(posx + anchoImagenX, posy, 0.1);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texFondo);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.0, alto, 0.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(ancho, alto, 0.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(ancho, 0.0, 0.0);
	glEnd();
	
	glFlush();
	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
	
	}
	
	void reshape(int w, int h){
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (GLfloat) w, 0, (GLfloat) h, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	ancho = w;
	alto = h;}
	
	void mueveImagen (void){
			
		theta += deltaTheta;
		r = r0 + k*(theta);
		posx = r*cos(theta) + (ancho - anchoImagenX)/2;
		posy = r*sin(theta) + (alto - altoImagenY)/2;
		glutPostRedisplay();
		
		if ((r + 375) >= alto) deltaTheta *= -1; 
		if (r <= 0) deltaTheta *= -1;
	}

int main(int argc, char** argv)
	{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(800, 500);
   glutInitWindowPosition(283, 134);
   glutCreateWindow(argv[0]);
   init();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutIdleFunc(mueveImagen);
   glutMainLoop();
   return 0; 
   }

