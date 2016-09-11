

 //#ifdef USE_PRECOMPILED_HEADERS
 #include <string>
 #include <sstream>
 #include <iostream>
 //#include <stdafx.h>
 //#endif
 #include <GL/glut.h>
 #include <stdlib.h>
 #include <string.h>
 #include <iostream>
 #include <GL/glut.h>


//#pragma comment (lib,"opengl32.lib")
//#pragma comment (lib,"glu32.lib")
//#pragma comment (lib,"glut32.lib")	

//Declaraciòn de los valores iniciales de las variables de posiciòn de las lìneas en Y:

int posyL1 = 275, posyL2 = 215, posyL3 = 175, posyL4 = 135, posyL5 = 95, posyL6 = 55 ,posyL7 = 15, ancho = 100, alto = 100;

void drawBitmapText(char *string,float x,float y,float z) 
{  
	char *c;
	glRasterPos3f(x,y,z);

	for (c=string; *c != '\0'; c++) 
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *c);
	}
}

void drawStrokeText(char *string,int x,int y,int z)
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

void init()
{
	glClearColor(0.0,0.0,0.0,0.0); 
	
}

void reshape(int w,int h) 
{ 
 glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (GLfloat) w, 0, (GLfloat) h, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	ancho = w;
	alto = h;

}

void mueveletras(){
	float deltaY = 1;
	posyL1 += deltaY;
	posyL2 += deltaY; 
	posyL3 += deltaY; 
	posyL4 += deltaY; 
	posyL5 += deltaY; 
	posyL6 += deltaY; 
	posyL7 += deltaY;
	glutPostRedisplay();
	}

void render(void)
{ 
	glClear(GL_COLOR_BUFFER_BIT); 
	glLoadIdentity();
	
	glutIdleFunc(mueveletras);
	if (posyL7 > alto){
		posyL1=0-40;
		posyL2=posyL1-80;
		posyL3=posyL2-40;
		posyL4=posyL3-40;
		posyL5=posyL4-40;
		posyL6=posyL5-40;
		posyL7=posyL6-40;} // se declara el salto a la finalizacion del programa
	glColor3f(0,1,0);
	drawStrokeText("GRACIAS POR JUGAR MICRONOID",474,posyL1,0);
	
	
	glColor3f(0,1,0);
	drawStrokeText("LUIS ALFARO ROJAS CARNET: 201214010",418,posyL2,0);
	
	glColor3f(0,1,0);
	drawStrokeText("DAVID MARIN SOTO CARNET: 201214031",460,posyL3,0);
	
	glColor3f(0,1,0);
	drawStrokeText("DANIEL ROJAS CHACON CARNET: 201246882",402,posyL4,0);
	
	glColor3f(0,1,0);
	drawStrokeText("DANIEL VIQUEZ GOMEZ CARNET: 201236251",402,posyL5,0);
	
	glColor3f(0,1,0);
	drawStrokeText("XXX",680,posyL6,0);
	
	glColor3f(0,1,0);
	drawStrokeText("PRESIONE ENTER PARA TERMINAR",466,posyL7,0);
	
	
	glutSwapBuffers(); 
	
} 

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
	  default:
		break;
   }
}

int main(int argc, char* argv[])
{
		// initialize glut 
        glutInit(&argc, argv); 
 
        // specify the display mode to be RGB and single buffering  
        // we use single buffering since this will be non animated 
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
        glutInitWindowSize(800,500); 
        glutInitWindowPosition(283,134); 
        glutCreateWindow("OpenGL Fonts"); 
		glutKeyboardFunc(keyboard);
        glutDisplayFunc(render);
        glutReshapeFunc(reshape);  
        glutMainLoop(); 
	return 0;
}
