
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <SOIL/SOIL.h>
#include <time.h>
#include <cmath>
#include <string>
#include <sstream>
#include <iostream>

//Variable de avance en el juego
static int avanza=0;

//Variables de texturas de los elementos del juego
static GLuint texName, texBola, texBloque,texbarra, texcora,texinicio,texGameover, texFondo,texGane, texFiesta;

//Valores iniciales de la pantalla
static GLint ancho = 100, alto = 100;
char nombre[100];
int indiceNombre = 0;
static int puede=0;

//Variables de posición de la bola
static GLint xBola = 360, yBola = 50, deltaX = -5, deltaY = 5;

//Variables de posición y existencia de los bloques
static GLboolean bloques[13][5];
static int bloquesRestantes = 5*13;
static int margen = 5, radio = 8;

//Variables de vidas restantes
static GLint vidas=3;

//Variables iniciales de la barra
static GLint posxbar1=320;
static GLint posxbar2=445;
static GLint posybar1=55;
static GLint posybar2=80;
static GLint anchobarra=125;

//Variables de pantalla al perder

static GLint anchoImagenX = 338, altoImagenY = 137;
float posx = 600;
float posy = 550;

float r = 0.78540;
float theta = 0.78540;
float r0 = 0;
float k = 7;
float deltaTheta = 0.08727;

//Variables de pantalla al ganar

typedef struct Bloque {
	float posx;
	float omega;
} Bloque;


static GLint tiempo = 0;
static Bloque bloquesg[] = { 
	{ 400, 0.10471 }, 
	{ 400, 0.06981 },
	{ 400, 0.05236 }, 
	{ 400, 0.04189 }, 
	{ 400, 0.08376 }
};

static GLint xDelta[] = {15, 10, 25, 30, 22};
static GLint xBloque[5] = {0, 0, 0, 0, 0};

//Variables de posición de créditos
int posyL1 = 275, posyL2 = 215, posyL3 = 175, posyL4 = 135, posyL5 = 95, posyL6 = 55 ,posyL7 = 15;

//*********************************************************************************************************************
//*****************Inicio de Código*************************************************************************************
//**********************************************************************************************************************


void drawBitmapText(char *string,float x,float y,float z) 
{  
	char *c;
	glRasterPos3f(x,y,z);

	for (c=string; *c != '\0'; c++) 
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *c);
	}
}

void drawStrokeText(char *string,float x,float y,float z)
{
	  char *c;
	  glPushMatrix();
	  glTranslatef(x, y+8,z);
	  glScalef(0.19f,0.18f,z);
  
	  for (c=string; *c != '\0'; c++)
	  {
    		glutStrokeCharacter(GLUT_STROKE_ROMAN , *c);
	  }
	  glPopMatrix();
}

//**********************************************************************************************************

//Movimiento de los créditos

void mueveletras(){
	float deltaY2 = 1;
	posyL1 += deltaY2;
	posyL2 += deltaY2; 
	posyL3 += deltaY2; 
	posyL4 += deltaY2; 
	posyL5 += deltaY2; 
	posyL6 += deltaY2; 
	posyL7 += deltaY2;
	glutPostRedisplay();
	}

//Movimiento de la pantalla al perder

void mueveImagen (void){
			
	theta += deltaTheta;
	r = r0 + k*(theta);
	posx = r*cos(theta) + (ancho - anchoImagenX)/2;
	posy = r*sin(theta) + (alto - altoImagenY)/2;
	glutPostRedisplay();
	
	if ((r + 375) >= alto) deltaTheta *= -1; 
	if (r <= 0) deltaTheta *= -1;
}

//Movimiento de bloques de pantalla de gane

void mueveBloques (void){
	
	//for (int i = 0; i < 5; i++) xBloque[i] += xDelta[i];
	
	for (int i = 0; i < 5; i++) {bloquesg[i].posx = (ancho - 350)*0.5*(1+cos(bloquesg[i].omega * tiempo));}
	
	glutPostRedisplay();
	for (int i = 0; i < 5; i++){
	if (xBloque[i] >= (ancho - 350)) xDelta[i] *= -1;
	if (xBloque[i] < 0) xDelta[i] *= -1;}
	
	tiempo++;	
}

//Sección de la pantalla inicial del juego

void init(void){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	//Textura pantalla de inicio
	texinicio = SOIL_load_OGL_texture(
		"fondogeneral.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);
	
	glBindTexture(GL_TEXTURE_2D, texinicio);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
	//Textura pantalla de perdedor
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
	
	//Textura pantalla gane
	
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
	
	//Textura fondo del juego
	texName = SOIL_load_OGL_texture(
		"fondogeneral(4).png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);
	
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
	//Textura de la barra
	texbarra = SOIL_load_OGL_texture(
		"barra.jpeg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);
	
	glBindTexture(GL_TEXTURE_2D, texbarra);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
	//Textura del corazón
	texcora = SOIL_load_OGL_texture(
		"corazon.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);
	
	glBindTexture(GL_TEXTURE_2D, texcora);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
	//Textura de la bola
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
		"bloque(2).jpeg",
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
	
	//Si aún está en la pantalla de inicio
	if(avanza==0){
		glBindTexture(GL_TEXTURE_2D, texinicio);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.0, alto, 0.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(ancho, alto, 0.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(ancho, 0.0, 0.0);
		glEnd();
		
		glDisable(GL_TEXTURE_2D);
		glColor3f(1,1,1);
		//drawStrokeText("BIENVENIDO A MICRONOID",410,550,0.2);
		drawStrokeText("Bienvenido a Micronoid",ancho*0.25,alto*0.76,0.2);
		
		glColor3f(1,1,1);
		drawStrokeText("Lab de Eestructura de Microprocesadores",ancho*0.12,alto*0.67,0.2);
		
		glColor3f(1,1,1);
		drawStrokeText("2do Semestre-2016",ancho*0.28,alto*0.62,0.2);
		
		glColor3f(1,1,1);
		drawStrokeText("Ingrese el nombre del jugador",ancho*0.21,alto*0.52,0.2);
		
		glColor3f(1,1,1);
		drawStrokeText("y presione enter",ancho*0.3,alto*0.45,0.2);
		
		glColor3f(1,1,1);
		drawStrokeText(nombre, ancho*0.35,alto*0.38,0.2);
		glEnable(GL_TEXTURE_2D);
	}
	
	//Si está en la pantalla de juego
	if(avanza==1){
		if(puede==0){
			glDisable(GL_TEXTURE_2D);
			/*glColor3f(1,1,1);
			glBegin(GL_QUADS);
				glVertex3f(400, 200, 0.0);
				glVertex3f(400, 240, 0.0);
				glVertex3f(750, 240, 0.0);
				glVertex3f(750, 200, 0.0);
			glEnd();*/
			glColor3f(0,0,1);
			drawStrokeText("PRESIONE X PARA INICIAR",ancho*0.3,alto*0.40,0.2);
			glEnable(GL_TEXTURE_2D);
			}
		
		//Impresión del nombre de usuario
		glDisable(GL_TEXTURE_2D);
		/*glColor3f(1,1,1);
		glBegin(GL_QUADS);
			glVertex3f(ancho-200, 10, 0.0);
			glVertex3f(ancho-200, 35, 0.0);
			glVertex3f(ancho-100, 35, 0.0);
			glVertex3f(ancho-100, 10, 0.0);
		glEnd();*/
		glColor3f(0,0,1);
		drawStrokeText(nombre, ancho-200,10,0.2);
		glEnable(GL_TEXTURE_2D);
		
		glBindTexture(GL_TEXTURE_2D, texName);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
			glTexCoord2f(0.0, 1.0); glVertex3f(0.0, alto, 0.0);
			glTexCoord2f(1.0, 1.0); glVertex3f(ancho, alto, 0.0);
			glTexCoord2f(1.0, 0.0); glVertex3f(ancho, 0.0, 0.0);
		glEnd();
		
		//Dibujar la bola
		glBindTexture(GL_TEXTURE_2D, texBola);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); glVertex3f(xBola, yBola, 0.2);
			glTexCoord2f(0.0, 1.0); glVertex3f(xBola, yBola + 20, 0.2);
			glTexCoord2f(1.0, 1.0); glVertex3f(xBola + 20, yBola + 20, 0.2);
			glTexCoord2f(1.0, 0.0); glVertex3f(xBola + 20, yBola, 0.2);
		glEnd();
		
		//Dibujar barra
		anchobarra=ancho*0.15625;
		posybar2=alto-(alto*0.9);
		posybar1=posybar2-25;
		glBindTexture(GL_TEXTURE_2D, texbarra);
		glBegin(GL_QUAD_STRIP);
			glTexCoord2f(0.0, 0.0); glVertex3f(posxbar1, posybar1, 0.2);
			glTexCoord2f(0.0, 1.0); glVertex3f(posxbar1, posybar2, 0.2);
			glTexCoord2f(1.0, 1.0); glVertex3f(posxbar1+anchobarra, posybar1, 0.2);
			glTexCoord2f(1.0, 0.0); glVertex3f(posxbar1+anchobarra, posybar2, 0.2);
		glEnd();
		
		//Dibujar corazones de las vidas
		glBindTexture(GL_TEXTURE_2D, texcora);
		int siguiente=0;
		for (int i=0; i<vidas; i++){
			glBegin(GL_QUAD_STRIP);
			glTexCoord2f(0.0, 0.0); glVertex3f(ancho-siguiente, 10, 0.2);
			glTexCoord2f(0.0, 1.0); glVertex3f(ancho-siguiente, 40, 0.2);
			glTexCoord2f(1.0, 1.0); glVertex3f(ancho-20-siguiente, 10, 0.2);
			glTexCoord2f(1.0, 0.0); glVertex3f(ancho-20-siguiente, 40, 0.2);
			glEnd();
			siguiente=siguiente+40;
		}
			
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
	}
	//Si perdió
	if(avanza==2){
		glDisable(GL_TEXTURE_2D);
		glColor3f(1,1,1);
		drawStrokeText("Presione enter para continuar",ancho-ancho*0.45,15,0);
		glEnable(GL_TEXTURE_2D);
		
		glutIdleFunc(mueveImagen);
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
		}
		
	//Si ganó
	if(avanza==3){
		glDisable(GL_TEXTURE_2D);
		glColor3f(0,0,0);
		drawStrokeText("PRESIONE ENTER PARA CONTINUAR",ancho-ancho*0.45,15,0);
		glEnable(GL_TEXTURE_2D);
		
		glutIdleFunc(mueveBloques);
		glBindTexture(GL_TEXTURE_2D, texGane);	
		int altoBloque = alto/5;
		int margenY = 30;
		
		for (int j = 0; j < 5; j++){
			//if (bloques[1][j])
				/*{*/ //for (int i = 0; i < 5; i++)	{
				
				glBegin(GL_QUADS);
					glTexCoord2f(0.0, 0.0); glVertex3f(bloquesg[j].posx, j*altoBloque + margenY, 0.1);
					glTexCoord2f(0.0, 1.0); glVertex3f(bloquesg[j].posx, (j + 1)*altoBloque, 0.1);
					glTexCoord2f(1.0, 1.0); glVertex3f(bloquesg[j].posx + 350,(j + 1)*altoBloque , 0.1);
					glTexCoord2f(1.0, 0.0); glVertex3f(bloquesg[j].posx + 350, j*altoBloque + margenY, 0.1);
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
		}
		
	//Ver créditos
	if(avanza==4){
		glBindTexture(GL_TEXTURE_2D, texinicio);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.0, alto, 0.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(ancho, alto, 0.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(ancho, 0.0, 0.0);
		glEnd();
		
		glDisable(GL_TEXTURE_2D);
		glutIdleFunc(mueveletras);
		if (posyL7 > alto){
			posyL1=0-40;
			posyL2=posyL1-80;
			posyL3=posyL2-40;
			posyL4=posyL3-40;
			posyL5=posyL4-40;
			posyL6=posyL5-40;
			posyL7=posyL6-60;} // se declara el salto a la finalizacion del programa
			
		glColor3f(1,1,1);
		drawStrokeText("GRACIAS POR JUGAR MICRONOID",ancho*0.25,posyL1,0.1);
		
		
		glColor3f(1,1,1);
		drawStrokeText("LUIS ALFARO ROJAS CARNET: 201214010",ancho*0.2,posyL2,0.1);
		
		glColor3f(1,1,1);
		drawStrokeText("DAVID MARIN SOTO CARNET: 201214031",0.2*ancho,posyL3,0.1);
		
		glColor3f(1,1,1);
		drawStrokeText("DANIEL ROJAS CHACON CARNET: 201246882",ancho*0.2,posyL4,0.1);
		
		glColor3f(1,1,1);
		drawStrokeText("DANIEL VIQUEZ GOMEZ CARNET: 201236251",ancho*0.2,posyL5,0.1);
		
		glColor3f(1,1,1);
		drawStrokeText("XXX",ancho*0.5,posyL6,0.1);
		
		glColor3f(1,1,1);
		drawStrokeText("PRESIONE ENTER PARA TERMINAR",ancho*0.24,posyL7,0.1);
		glEnable(GL_TEXTURE_2D);
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
   //int xBolaant=xBola;
   if((puede==0)&&(vidas!=3)){
	   xBola +=0;
	   yBola +=0;}
	   
   else{
   xBola += deltaX;
   yBola += deltaY;}
   int mitadbarra=((posxbar2-posxbar1-1)/2);
   //int altbarra=(posybar2-posybar1);
   
   glutPostRedisplay();
   
   //Comprobación de choque de la bola con las paredes del área de juego
   if (xBola >= (ancho - 20)) deltaX *= -1;
   if (yBola >= (alto - 20)) deltaY *= -1;
   if (xBola < 0) deltaX *= -1;
   if (yBola < 0) {deltaY *= -1; vidas=vidas-1; puede=0; xBola = (ancho*0.5-ancho*0.01); yBola = posybar2; posxbar1=(ancho*0.5-ancho*0.1);}
   if (vidas==0){avanza=2;} //Indica que el usuario perdió
   if (bloquesRestantes==0){avanza=3;}//Indica que el usuario ganó
   
   //Comprobación de choque de la bola con la barra
   if ((yBola == posybar2)&&(((xBola<(posxbar1+mitadbarra))&&(xBola>posxbar1))||((xBola>(posxbar2-mitadbarra))&&(xBola<posxbar2)))){
	   deltaY *= -1; 
   }
   
   if ((yBola == posybar2)&&((xBola==posxbar2)||(xBola==posxbar1))){
	   deltaY *= -1; 
	   deltaX *= -1;
   }
   
   if ((yBola < posybar2)&&((xBola==posxbar1)||((xBola)==posxbar2))){
	   deltaX *= -1;
   }

   //Comprobación de choque de la barra con los bloques
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
	if(avanza==0){
		if (((key >= 48) && (key <= 90)) || ((key >= 97) && (key <= 122)) || (key == 32)) {
			nombre[indiceNombre++] = key;
			nombre[indiceNombre] = '\0';}
	   
		else if (key == 8){
			nombre[--indiceNombre] = '\0';}
		else if (key == 13){
			avanza=1;}
		glutPostRedisplay();}
	
	if(avanza!=0){
	   switch (key) {
		  case 'x':
			if(avanza==1){
				puede=1;
				glutIdleFunc(mueveBola);}
				break;
		  case 'd':
			if((avanza==1)&&(puede==1)){
				anchobarra=posxbar2-posxbar1;
				if(posxbar1<=(ancho - anchobarra)){
					posxbar1=posxbar1+10;
					posxbar2=posxbar1+anchobarra;
					glutPostRedisplay();}}
					break;
		  case 'a':
			if((avanza==1)&&(puede==1)){
				anchobarra=posxbar2-posxbar1;
				if(posxbar1>=0.0){
					posxbar2=posxbar2-10;
					posxbar1=posxbar2-anchobarra;
					glutPostRedisplay();}}
					break;
		  case 13:
		   if(avanza==4){
			   exit(0);}
		   if((avanza==3)||(avanza==2)){
				avanza=4;
				glutPostRedisplay();
				 }
				break;
		  default:
			 break;
	   }
}
}	
//Código principal del programa

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(800, 500);
   glutInitWindowPosition(283, 134);
   glutCreateWindow("Micronoid");
   init();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0; 
}
