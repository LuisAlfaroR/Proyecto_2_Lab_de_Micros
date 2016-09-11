//#ifdef USE_PRECOMPILED_HEADERS
 #include <string>
 #include <sstream>
 #include <iostream>
 //#include <stdafx.h>
 //#endif
 #include <stdlib.h>
 #include <string.h>
 #include <iostream>
 #include <GL/glut.h>


//#pragma comment (lib,"opengl32.lib")
//#pragma comment (lib,"glu32.lib")
//#pragma comment (lib,"glut32.lib")	

char nombre[100];
int indiceNombre = 0;

void drawBitmapText(char *string,float x,float y,float z) 
{  
	char *c;
	glRasterPos3f(x,y,z);

	for (c=string; *c != '\0'; c++) 
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *c);
	}
}

void drawStrokeText(char*string,int x,int y,int z)
{
	  char *c;
	  glPushMatrix();
	  glTranslatef(x, y+8,z);
	  glScalef(0.19f,-0.18f,z);
  
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

void keyboard(unsigned char key, int x, int y)
{
   /*switch (key) {
      case 27:
         exit(0);
   }*/
   
   if (((key >= 48) && (key <= 90)) || ((key >= 97) && (key <= 122)) || (key == 32)) {
	   nombre[indiceNombre++] = key;
	   nombre[indiceNombre] = '\0';
       
   }
   else if (key == 8) {
	   nombre[--indiceNombre] = '\0';
   }
   else if (key == 13) {
	   //Falta implementar el paso de pantalla
	   
	   //cout << "Tecla enter" <<endl;
   }
   
   glutPostRedisplay();
}

void reshape(int w,int h) 
{ 
 
    glViewport(0,0,w,h); 
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 
    gluOrtho2D(0,w,h,0); 
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity(); 

}


void render(void)
{ 
	glClear(GL_COLOR_BUFFER_BIT); 
	glLoadIdentity();
 
	glColor3f(0,1,0);
	drawStrokeText("BIENVENIDO A MICRONOID",512,91,0);
	
	
	glColor3f(0,1,0);
	drawStrokeText("LAB DE ESTRUCTURA DE MICROPROCESADORES",393,182,0);
	
	glColor3f(0,1,0);
	drawStrokeText("2 SEMESTRE-2016",566,273,0);
	
	glColor3f(0,1,0);
	drawStrokeText("INGRESE EL NOMBRE DEL JUGADOR",460,364,0);
	
	glColor3f(0,1,0);
	drawStrokeText(nombre, 600,455,0);

	glutSwapBuffers(); 
} 



int main(int argc, char* argv[])
{
		// initialize glut 
        glutInit(&argc, argv); 
 
        // specify the display mode to be RGB and single buffering  
        // we use single buffering since this will be non animated 
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); 
 
        // define the size 
        glutInitWindowSize(800,500); 
 
        // the position where the window will appear 
        glutInitWindowPosition(283,134); 
        glutCreateWindow("OpenGL Fonts"); 
		glutKeyboardFunc(keyboard);
        glutDisplayFunc(render);
		 glutIdleFunc(render);
        glutReshapeFunc(reshape); 

        // enter the main loop 
        glutMainLoop(); 
	return 0;
}
