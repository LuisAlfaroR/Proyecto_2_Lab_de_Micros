#include <X11/Xlib.h>
#include <GL/glx.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <SOIL/SOIL.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>

typedef struct Bloque {
	float posx;
	float omega;
} Bloque;


static GLuint texGane, texFiesta;
static GLint ancho = 100, alto = 100, tiempo = 0;
static Bloque bloques[] = { 
	{ 400, 0.10471 }, 
	{ 400, 0.06981 },
	{ 400, 0.05236 }, 
	{ 400, 0.04189 }, 
	{ 400, 0.08376 }
};


//static GLboolean bloques[1][5];



static GLint xDelta[] = {15, 10, 25, 30, 22};
static GLint xBloque[5] = {0, 0, 0, 0, 0};


void init(void){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

texGane = SOIL_load_OGL_texture(
		"ganaste.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);
	
	glBindTexture(GL_TEXTURE_2D, texGane);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
	/*for (int j = 0; j < 5; j++) {
	bloques[1][j] = GL_TRUE;
	}*/
	
	texFiesta = SOIL_load_OGL_texture(
		"Fieston.PNG",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);
	
	glBindTexture(GL_TEXTURE_2D, texFiesta);
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
	
	glBindTexture(GL_TEXTURE_2D, texGane);
	int altoBloque = alto/5;
	int margenY = 30;
	
	for (int j = 0; j < 5; j++){
		//if (bloques[1][j])
			/*{*/ //for (int i = 0; i < 5; i++)	{
			
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0); glVertex3f(bloques[j].posx, j*altoBloque + margenY, 0.1);
				glTexCoord2f(0.0, 1.0); glVertex3f(bloques[j].posx, (j + 1)*altoBloque, 0.1);
				glTexCoord2f(1.0, 1.0); glVertex3f(bloques[j].posx + 350,(j + 1)*altoBloque , 0.1);
				glTexCoord2f(1.0, 0.0); glVertex3f(bloques[j].posx + 350, j*altoBloque + margenY, 0.1);
			glEnd();
			//}
		}
	//}
		
		
	
	glBindTexture(GL_TEXTURE_2D, texFiesta);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(0.0, 2.0); glVertex3f(0.0, alto, 0.0);
		glTexCoord2f(2.0, 2.0); glVertex3f(ancho, alto, 0.0);
		glTexCoord2f(2.0, 0.0); glVertex3f(ancho, 0.0, 0.0);
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
	
	void mueveBloques (void){
		
		//for (int i = 0; i < 5; i++) xBloque[i] += xDelta[i];
		
		for (int i = 0; i < 5; i++) bloques[i].posx = (ancho - 350)*0.5*(1+cos(bloques[i].omega * tiempo));
		
		glutPostRedisplay();
		for (int i = 0; i < 5; i++){
		if (xBloque[i] >= (ancho - 350)) xDelta[i] *= -1;
		if (xBloque[i] < 0) xDelta[i] *= -1;}
		
		tiempo++;	
	}
	
	/*void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case 32:
         glutIdleFunc(mueveBloques);
         break;
      case 27:
         glutIdleFunc(NULL);
         break;
      default:
         break;
   }
}*/
	
	
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
   glutIdleFunc(mueveBloques);
   glutMainLoop();
   return 0; 
   }

