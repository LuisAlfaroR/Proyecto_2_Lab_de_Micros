
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <SOIL/SOIL.h>
#include <stdlib.h>
#include <time.h>

static GLuint texName, texBola, texBloque;
static GLint ancho = 100, alto = 100;
static GLint xBola = 250, yBola = 50, deltaX = -5, deltaY = 5;
static GLboolean bloques[13][5];
static int bloquesRestantes = 5*13;
static int margen = 5, radio = 8;

void init(void){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	texName = SOIL_load_OGL_texture(
		"fondogeneral.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);
	
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
	texBola = SOIL_load_OGL_texture(
		"esfera.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);
	
	glBindTexture(GL_TEXTURE_2D, texBola);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
	texBloque = SOIL_load_OGL_texture(
		"bloque.jpeg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);
	
	glBindTexture(GL_TEXTURE_2D, texBloque);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 5; j++) {
			bloques[i][j] = GL_TRUE;
		}
	}
}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	/*
	glBindTexture(GL_TEXTURE_2D, texName);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.0, alto, 0.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(ancho, alto, 0.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(ancho, 0.0, 0.0);
	glEnd();*/
	
	
	glBindTexture(GL_TEXTURE_2D, texBola);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(xBola, yBola, 0.2);
		glTexCoord2f(0.0, 1.0); glVertex3f(xBola, yBola + 20, 0.2);
		glTexCoord2f(1.0, 1.0); glVertex3f(xBola + 20, yBola + 20, 0.2);
		glTexCoord2f(1.0, 0.0); glVertex3f(xBola + 20, yBola, 0.2);
	glEnd();
	
	//Dibujar bloques
	glBindTexture(GL_TEXTURE_2D, texBloque);
	
	int anchoBloque = ancho / 13;
	int altoBloque = alto / (5 * 4);
	int margen = 5;
	int offsetY = (3 * alto) / 4;
	
	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 5; j++) {
			if (bloques[i][j]) {
				glBegin(GL_QUADS);
					glTexCoord2f(0.0, 0.0); glVertex3f(i * anchoBloque + margen, j*altoBloque+margen + offsetY, 0.1);
					glTexCoord2f(0.0, 1.0); glVertex3f(i * anchoBloque + margen, (j+1)*altoBloque - margen + offsetY, 0.1);
					glTexCoord2f(1.0, 1.0); glVertex3f((i + 1) * anchoBloque - margen, (j+1)*altoBloque - margen + offsetY, 0.1);
					glTexCoord2f(1.0, 0.0); glVertex3f((i + 1) * anchoBloque - margen, j*altoBloque + margen + offsetY, 0.1);
				glEnd();
			}
		}
	}
	
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
	alto = h;
}

void mueveBola(void)
{	
   xBola += deltaX;
   yBola += deltaY;
   
   glutPostRedisplay();
   
   if (xBola >= (ancho - 20)) deltaX *= -1;
   if (yBola >= (alto - 20)) deltaY *= -1;
   if (xBola < 0) deltaX *= -1;
   if (yBola < 0) deltaY *= -1;
   
   for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 5; j++) {
			
			if (!bloques[i][j]) {
				continue;
			}
			
			int centroX = xBola + 10;
			int centroY = yBola + 10;
			int xBloque = (ancho / 13) * i + margen;
			int yBloque = alto * j / 20 + (3 * alto) / 4 + margen;
			int anchoBloque = (ancho / 13) - 2*margen;
			int altoBloque = (alto / 20) - 2*margen;
			
			GLboolean huboColision = GL_FALSE;
			
			//Comprobar colision de la bola con el bloque (general)
			if (!((centroX > (xBloque - radio)) && (centroX < (xBloque + anchoBloque + radio)) && 
			      (centroY > (yBloque - radio)) && (centroY < (yBloque + altoBloque + radio)))) {
					  
				continue;
		    }
			
			int distanciaLadoIzquierdo = xBloque - centroX;
			int distanciaLadoDerecho = centroX - (xBloque + anchoBloque);
			int distanciaLadoArriba = centroY - (yBloque + altoBloque);
			int distanciaLadoAbajo = yBloque - centroY;
			
			if ((deltaX > 0) && (distanciaLadoIzquierdo >= 0) && (distanciaLadoIzquierdo < radio)) {
				deltaX *= -1;
				huboColision = GL_TRUE;	
			}
			if ((deltaX < 0) && (distanciaLadoDerecho >= 0) && (distanciaLadoDerecho < radio)) {
				deltaX *= -1;
				huboColision = GL_TRUE;
			}
			if ((deltaY > 0) && (distanciaLadoAbajo >= 0) && (distanciaLadoAbajo < radio)) {
				deltaY *= -1;
				huboColision = GL_TRUE;	
			}
			if ((deltaY < 0) && (distanciaLadoArriba >= 0) && (distanciaLadoArriba < radio)) {
				deltaY *= -1;
				huboColision = GL_TRUE;	
			}
			
			if (huboColision) {
				bloquesRestantes--;
				bloques[i][j] = GL_FALSE;
				return;
			}
			
		}
   }
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case 32:
         glutIdleFunc(mueveBola);
         break;
      case 27:
         glutIdleFunc(NULL);
         break;
      default:
         break;
   }
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
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0; 
}
