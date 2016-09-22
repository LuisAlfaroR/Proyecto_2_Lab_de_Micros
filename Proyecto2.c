
//*****************************************************************************************************************************************
//Se incluyen las librerias necesarias para poder ejecutar correctamente
//el programa, incuyendo algunas librerias que normalmente se usan en la
//programacion en c y otras no muy usuales que se usan en la programacion
//grafica con OpenGl, que son las librerias utilizadas para implementar
//el programa de forma grafica.
//*****************************************************************************************************************************************   

#include <errno.h>
#include <wiringPi.h>  //Libreria para utilizar pines GPIO del Raspberry
#include <softTone.h> //Libreria para generar señal Beep

#include <X11/Xlib.h>
#include <GL/glx.h> //1ª Libreria de OpenGL, se escribe GL/ para especificar la ubicacion en el sistema de el archivo glx.h
#include <GL/glut.h>//2ª Libreria de OpenGL
#include <stdlib.h>
#include <stdio.h>
#include <SOIL/SOIL.h>// Libreria utilizada para facilitar el manejo de las imagenes en las texturas 
#include <time.h>
#include <cmath>
#include <string>
#include <sstream>
#include <iostream>

#define	PIN 0  //Pin GPIO17 donde se genera señal

int reset=0;  //Variable para resetear juego

//Variable de avance en el juego
static int avanza=0; //El valor de esta variable permite controlar el 
                     //avance del juego en forma correcta, entre las 
                     //diferentes pantallas segun las condiciones actuales del juego

//Variables de texturas de los elementos del juego
static GLuint texName, texBola, texBloque,texbarra, texcora,texinicio,texGameover, texFondo,texGane, texFiesta;

//Valores iniciales de la pantalla
static GLint ancho = 100, alto = 100;
char nombre[100];      //esto crea una variable char que almacena caracteres y le da 100 espacios al jugador para poner su nombre. 
int indiceNombre = 0;  //controla la posicion de la letra de entrada dentro del char "nombre".
static int puede=0;    //variable que controla el avance del juego.

//Variables de posición de la bola:
static GLint xBola = 360, yBola = 80, deltaX = -5, deltaY = 5;

//Variables de posición y existencia de los bloques:
static GLboolean bloques[13][5]; //Inicializa un arreglo de booleanos que se asignara luego a los bloques para poder dibujarlos 
static int bloquesRestantes = 5*13; //Calcula cuantos bloques hay inicialmente
static int margen = 5, radio = 8; 

//Variables de vidas restantes
static GLint vidas=3; 

//Variables iniciales de la posicion de la barra en "X" e "Y", asi como la variable de ancho de la barra en pixeles que se usa despues
static GLint posxbar1=320;
static GLint posxbar2=445;
static GLint posybar1=50;
static GLint posybar2=80;
static GLint anchobarra=10;

//Variables de la pantalla al perder, definen el tamano de la imagen de "Game Over" y las posiciones que luego se cambian
//para lograr el efecto de movimiento en la imagen, al final la posicion de la imagen se asigna a posx y posy  

static GLint anchoImagenX = 338, altoImagenY = 137;
float posx = 600;
float posy = 550;

float r = 0.78540;
float theta = 0.78540;
float r0 = 0;
float k = 7;
float deltaTheta = 0.08727;

//Variables de pantalla al ganar

typedef struct Bloque {  //se crea un struct para facilitar la creacion de varios elementos
	float posx;          //iguales que tienen variables de posicion en el eje x y un valor
	float omega;		 //omega que controla la velocidad de movimiento de los bloques de
						 //ganaste en la pantalla de gane
} 

Bloque;				           //Se define para 5 elementos del tipo Bloque los valores de posicion inicial de 
static GLint tiempo = 0;	   // "x" y el valor de la velocidad angular omega, esto para permitir que los  
static Bloque bloquesg[] = {   //bloques de "Ganaste!" se muevan a diferentes velocidades. Todos los bloques
	{ 400, 0.10471 }, 		   //inician en una posicion inicial de 400 para considerar el tamaño del bloque en pixeles
	{ 400, 0.06981 },
	{ 400, 0.05236 }, 
	{ 400, 0.04189 }, 
	{ 400, 0.08376 }
};

static GLint xDelta[] = {15, 10, 25, 30, 22}; //establece la velocidad de avance para cada bloque de "Ganaste!" en la animacion correspondiente
static GLint xBloque[5] = {0, 0, 0, 0, 0};

//Variables de posición de créditos
int posyL1 = 275, posyL2 = 215, posyL3 = 175, posyL4 = 135, posyL5 = 95, posyL6 = 55 ,posyL7 = 15; //variables para ubicar las
																							//posiciones en el eje "y" iniciales
	
																						  //para cada linea en la pantalla final o creditos 

//*********************************************************************************************************************
//*****************Inicio de Código*************************************************************************************
//**********************************************************************************************************************

void reiniciar()  //	Funcion Reiniciar, lo que hace es reiniciar los valores  del juego
					//Restablece bloques, vidas, posicion de barra y bola y el nombre del jugador
{
		avanza=0;
		vidas=3; puede=0; xBola = (ancho*0.5); yBola = posybar2+5; posxbar1=(ancho*0.5-ancho*0.1);
		bloquesRestantes = 5*13;indiceNombre = 0; 
		for(int i = 0; i < 101; i++){
		nombre[i]='\0';
		}
		for (int i = 0; i < 13; i++) {
			for (int j = 0; j < 5; j++) {
				if (bloques[i][j]==0) {
					bloques[i][j]=1;
					}}}
}

void tono()  //Genera la señal del beep utilizando la libreria WiringPI
{

  softToneCreate (PIN) ;  //Inicializa el pin por donde se envia la señal
  softToneWrite (PIN,500) ;  //Se le dice la frecuencia del tono
  delay (100) ;            //DElay para la duracion del beep
  softToneWrite (PIN,0) ;   //Resetea el beep
  
   
  }

void drawStrokeText(char *string,float x,float y,float z)  //La funcion drawStrokeText me permite ingresar texto en pantalla usando
{														   //algunas de las caracteristicas de OpenGL
	  char *c;
	  glPushMatrix();  //Permite empujar al contenido de la matriz de la pila del sistema
	  glTranslatef(x, y+8,z); //Permite ubicar los valores de escritura mediante una multiplicacion entre la matriz actual y la de traslacion 
	  glScalef(0.19f,0.18f,z);  //Permite escalar el tamaño del texto y lo hace multiplicando la matriz actual por una matriz de escalado
  
	  for (c=string; *c != '\0'; c++) //Permite ir avanzando en las posiciones de escritura
	  {
    		glutStrokeCharacter(GLUT_STROKE_ROMAN , *c);  //permite devolver un caracter que se presiona en el teclado usando OpenGL
	  }
	  glPopMatrix();  //empuja y saca la matriz de la pila del sistema.  
}

//**********************************************************************************************************

//Movimiento de los créditos

void mueveletras(){		//Esta funcion se encarga de darle movimiento a las lineas de los creditos cambiando constantemente sus valores 
	float deltaY2 = 1;	//en las posiciones de "Y" de cada linea. Estos cambios son iguales en valor para cada linea de forma que 
	posyL1 += deltaY2;	//se muevan de la misma manera todas las lineas. 
	posyL2 += deltaY2; 
	posyL3 += deltaY2; 
	posyL4 += deltaY2; 
	posyL5 += deltaY2; 
	posyL6 += deltaY2; 
	posyL7 += deltaY2;
	glutPostRedisplay();
	}

//Movimiento de la pantalla al perder

void mueveImagen (void){ 		//Esta funcion emula el comportamiento matematico de la espiral de Arquimides
								
	theta += deltaTheta;
	r = r0 + k*(theta); 		//Funcion que permite obtener una espiral
	posx = r*cos(theta) + (ancho - anchoImagenX)/2; //Conversion a la componente en x de la espiral
	posy = r*sin(theta) + (alto - altoImagenY)/2;  ////Conversion a la componente en y de la espiral
	glutPostRedisplay();	//Funcion de "repintado", que permite que se hagan los cambios en pantalla
	
	if ((r + 375) >= alto) deltaTheta *= -1; 	//Comprueba que el valor de "r" no supere el limite de 375 para evitar que la 
	if (r <= 0) deltaTheta *= -1;				//imagen en la espiral se salga del campo de visualizacion en la pantalla, y en 
}												//caso de llegar a ese valor entonces se cambia el sentido de giro de la espiral
												//con el signo de "deltaTheta". Tambien se varia el sentido si r <= 0 se cumple
//Movimiento de bloques de pantalla de gane

void mueveBloques (void){		//Esta funcion se encarga de emular para cada uno de los 5 bloques del mensaje "Ganaste!" un movimiento
								//armonico simple, estos bloques se mueven a diferente velocidad gracias al valor de omega individual    
	for (int i = 0; i < 5; i++) {bloquesg[i].posx = (ancho - 350)*0.5*(1+cos(bloquesg[i].omega * tiempo));} //asignado anteriormente.
	
	glutPostRedisplay();
	for (int i = 0; i < 5; i++){						//Este ciclo for permite comprobar que los bloques de Ganaste! estàn dentro de la 
	if (xBloque[i] >= (ancho - 350)) xDelta[i] *= -1;	//pantalla y cambia el sentido en que avanzan cuando logra comprobar que se llega 
	if (xBloque[i] < 0) xDelta[i] *= -1;}				//al borde. 
	
	tiempo++;	
}

//Sección de la pantalla inicial del juego

void init(void){
	glClearColor(0.0, 0.0, 0.0, 0.0);		//Especifica colores claros a los buffers de color, los valores iniciales son ceros siempre
	glShadeModel(GL_FLAT);					//Permite escoger el tipo de sombreado, ya sea plano o liso, que se le dara a las texturas 
	glEnable(GL_DEPTH_TEST);				//Esta funcion es realmente importante porque permite habilitar las funciones de GL en el servidor
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	//Para especificar los modos de almacenamiento del conjunto de pixeles. Con el argumento que 
											//tiene esta funcion especifica los requisitos de alineacion para el inicio de cada fila de 
											//pixeles en la memoria. Con 1 permite la alineacion de 2 bytes. 
	//Textura pantalla de inicio
	texinicio = SOIL_load_OGL_texture(		//Permite cargar texturas desde imagenes desde varios formatos tales como JPEG, JPG, PNG, etc.  
		"fondogeneral.png",					//Especifica el nombre del archivo que sera usado como textura.
		SOIL_LOAD_AUTO,						//Se carga como una textura la imagen del fondo en pantalla de inicio.
		SOIL_CREATE_NEW_ID,				    //Se crea un nuevo identicador para la textura.
		SOIL_FLAG_INVERT_Y					//Voltea la imagen verticalmente.
	);
	
	glBindTexture(GL_TEXTURE_2D, texinicio);						//Une a la textura texinicio llamada a una texturizacion en 2D
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);		
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
	//Textura pantalla de perdedor
	texGameover = SOIL_load_OGL_texture(	//Permite cargar texturas desde imagenes desde varios formatos tales como JPEG, JPG, PNG, etc. 
		"gameoficial.PNG",					//Especifica el nombre del archivo que sera usado como textura.
		SOIL_LOAD_AUTO,						//Se carga como una textura la imagen del fondo en pantalla de inicio.
		SOIL_CREATE_NEW_ID,					//Se crea un nuevo identicador para la textura.
		SOIL_FLAG_INVERT_Y					//Voltea la imagen verticalmente.
	);

	
	glBindTexture(GL_TEXTURE_2D, texGameover);						//Une a la textura texGameover llamada a una texturizacion en 2D
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
	texFondo = SOIL_load_OGL_texture(		//Permite cargar texturas desde imagenes desde varios formatos tales como JPEG, JPG, PNG, etc. 
		"GameOvFondo2.PNG",					//Especifica el nombre del archivo que sera usado como textura.
		SOIL_LOAD_AUTO,						//Se carga como una textura la imagen del fondo en pantalla de inicio.
		SOIL_CREATE_NEW_ID,					//Se crea un nuevo identicador para la textura.
		SOIL_FLAG_INVERT_Y					//Voltea la imagen verticalmente.
	);
	
	glBindTexture(GL_TEXTURE_2D, texFondo);							//Une a la textura texFondo llamada a una texturizacion en 2D
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
	//Textura pantalla gane
	
	texGane = SOIL_load_OGL_texture(		//Permite cargar texturas desde imagenes desde varios formatos tales como JPEG, JPG, PNG, etc. 
		"ganaste.png",						//Especifica el nombre del archivo que sera usado como textura.
		SOIL_LOAD_AUTO,						//Se carga como una textura la imagen del fondo en pantalla de inicio.
		SOIL_CREATE_NEW_ID,					//Se crea un nuevo identicador para la textura.
		SOIL_FLAG_INVERT_Y					//Voltea la imagen verticalmente.
	);
	
	glBindTexture(GL_TEXTURE_2D, texGane);							//Une a la textura texGane llamada a una texturizacion en 2D
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
	texFiesta = SOIL_load_OGL_texture(		//Permite cargar texturas desde imagenes desde varios formatos tales como JPEG, JPG, PNG, etc. 
		"Fieston.PNG",						//Especifica el nombre del archivo que sera usado como textura.
		SOIL_LOAD_AUTO,						//Se carga como una textura la imagen del fondo en pantalla de inicio.
		SOIL_CREATE_NEW_ID,					//Se crea un nuevo identicador para la textura.
		SOIL_FLAG_INVERT_Y					//Voltea la imagen verticalmente.
	);
	
	glBindTexture(GL_TEXTURE_2D, texFiesta);						//Une a la textura texFiesta llamada a una texturizacion en 2D
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
	//Textura fondo del juego
	texName = SOIL_load_OGL_texture(		//Permite cargar texturas desde imagenes desde varios formatos tales como JPEG, JPG, PNG, etc. 
		"fondoGeneral.jpg",					//Especifica el nombre del archivo que sera usado como textura.
		SOIL_LOAD_AUTO,						//Se carga como una textura la imagen del fondo en pantalla de inicio.
		SOIL_CREATE_NEW_ID,					//Se crea un nuevo identicador para la textura.
		SOIL_FLAG_INVERT_Y					//Voltea la imagen verticalmente.
	);
	
	glBindTexture(GL_TEXTURE_2D, texName);							//Une a la textura texName llamada a una texturizacion en 2D
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
	//Textura de la barra
	texbarra = SOIL_load_OGL_texture(		//Permite cargar texturas desde imagenes desde varios formatos tales como JPEG, JPG, PNG, etc. 
		"barra.jpeg",						//Especifica el nombre del archivo que sera usado como textura.
		SOIL_LOAD_AUTO,						//Se carga como una textura la imagen del fondo en pantalla de inicio.
		SOIL_CREATE_NEW_ID,					//Se crea un nuevo identicador para la textura.
		SOIL_FLAG_INVERT_Y					//Voltea la imagen verticalmente.
	);
		
	glBindTexture(GL_TEXTURE_2D, texbarra);						//Une a la textura texbarra llamada a una texturizacion en 2D
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
	//Textura del corazón
	texcora = SOIL_load_OGL_texture(		//Permite cargar texturas desde imagenes desde varios formatos tales como JPEG, JPG, PNG, etc. 
		"CoRazon.png",						//Especifica el nombre del archivo que sera usado como textura.
		SOIL_LOAD_AUTO,						//Se carga como una textura la imagen del fondo en pantalla de inicio.
		SOIL_CREATE_NEW_ID,					//Se crea un nuevo identicador para la textura.
		SOIL_FLAG_INVERT_Y					//Voltea la imagen verticalmente.
	);
	
	glBindTexture(GL_TEXTURE_2D, texcora);						//Une a la textura texcora llamada a una texturizacion en 2D
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
	//Textura de la bola
	texBola = SOIL_load_OGL_texture(		//Permite cargar texturas desde imagenes desde varios formatos tales como JPEG, JPG, PNG, etc. 
		"esfera.png",						//Especifica el nombre del archivo que sera usado como textura.
		SOIL_LOAD_AUTO,						//Se carga como una textura la imagen del fondo en pantalla de inicio.
		SOIL_CREATE_NEW_ID,					//Se crea un nuevo identicador para la textura.
		SOIL_FLAG_INVERT_Y					//Voltea la imagen verticalmente.
	);
	
	glBindTexture(GL_TEXTURE_2D, texBola);						//Une a la textura texBola llamada a una texturizacion en 2D
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
	texBloque = SOIL_load_OGL_texture(		//Permite cargar texturas desde imagenes desde varios formatos tales como JPEG, JPG, PNG, etc. 
		"bloque(2).jpeg",					//Especifica el nombre del archivo que sera usado como textura.
		SOIL_LOAD_AUTO,						//Se carga como una textura la imagen del fondo en pantalla de inicio.
		SOIL_CREATE_NEW_ID,					//Se crea un nuevo identicador para la textura.
		SOIL_FLAG_INVERT_Y					//Voltea la imagen verticalmente.
	);
	
	glBindTexture(GL_TEXTURE_2D, texBloque);					//Une a la textura texBloque llamada a una texturizacion en 2D
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	for (int i = 0; i < 13; i++) {			//Este ciclo se encarga de inicializar los valores de los booleanos que identificaran a cada 
		for (int j = 0; j < 5; j++) {		//bloque como 1 o encendidos, mediante GL_TRUE expresion que no corresponde a un booleano en
			bloques[i][j] = GL_TRUE;		//todo el sentido de la plalabra pero que puede usarse en este contexto. 
		}
	}
}

//Esta funcion es la que tiene la importante tarea manejar lo que se esta imprimiendo en pantalla, se llama 1 vez segun sea 
//necesario para cada etapa del juego y su avance. Es por esto que se hacen una serie de comprobaciones antes de decidir cual 
//codigo especificamente es necesario ejecutar.  
void display(void){	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpia los buffers con los valores preestablecidos.
	glEnable(GL_TEXTURE_2D);	//Esta funcion es realmente importante porque permite habilitar las funciones de GL en el servidor.
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); //permite establecer los parametros especificados de entorno y textura.
	glEnable(GL_BLEND);			//Habilita la caracteristica especificada en los parentesis propia de OpenGL.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//Especifica como deben dibujarse los pixeles usando una funcion que mezcla los  
														//valores de RGBA entrantes y los que que ya estan almacenados en el "frame buffer"
	reset=digitalRead(3); //Hace lectura del Pin GPIO18 para resetear el juego
	if(reset==1){
		reiniciar();  //Ejecuta el reseteo de las pantallas
		glutPostRedisplay();} //Vuelve a imprimir la pantalla 
	//Si aún está en la pantalla de inicio
	if(avanza==0){										//Comprueba si actualmente el programa esta en pantalla de inicio, pues cumple avanza == 0.
		glBindTexture(GL_TEXTURE_2D, texinicio);		//Une a la textura texinicio llamada a una texturizacion en 2D
		glBegin(GL_QUADS);								//Permite especificar como se delimitan los vertices de una primitiva, usando un cuadrilatero en este caso
		glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);		//glTexCoord2f permite establecer los valores de las coordenadas actuales 
		glTexCoord2f(0.0, 1.0); glVertex3f(0.0, alto, 0.0);		//si se supera el valor entre 0 y 1 la textura se dibuja mas de una vez en pantalla.
		glTexCoord2f(1.0, 1.0); glVertex3f(ancho, alto, 0.0);	//glVertex3f especifica donde se colocan los vertices de la imagen que es textura. 
		glTexCoord2f(1.0, 0.0); glVertex3f(ancho, 0.0, 0.0);	
		glEnd();												//glEnd permite cerrar las especificaciones abiertas en glBegin().
		
		glDisable(GL_TEXTURE_2D);								//Inhabilita temporalmente la caracteristica especificada en el argumento para la textura
												
		glColor3f(1,1,1);										//Especifica el color del texto que sera dibujado en formato R,G,B
		drawStrokeText("Bienvenido a Micronoid",512,455,0.2);	//Llama a la funcion drawStrokeText que permite dibujar texto mediante un "string".
																//y muestra es texto en pantalla. Ademas, los valores separados por comas 
		glColor3f(1,1,1);										//permiten establecer una ubicacion en x,y,z del texto que sera escrito.
		drawStrokeText("Lab de Estructura de Microprocesadores",393,364,0.2);	
		
		glColor3f(1,1,1);
		drawStrokeText("2do Semestre-2016",564,273,0.2);
		
		glColor3f(1,1,1);
		drawStrokeText("Ingrese el nombre del jugador y presione ENTER",400,182,0.2);
		
		glColor3f(1,1,1);
		drawStrokeText(nombre, 600,91,0.2);
		
		glEnable(GL_TEXTURE_2D);
	}
	
	//Si está en la pantalla de juego
	if(avanza==1){							//Comprueba si la pantalla actual es la corespondiente al area de juego verificando si avanza vale 1.
		if(puede==0){						//Comprueba si la persona presionó x para poder empezar a jugar
			glDisable(GL_TEXTURE_2D);		//Apaga temporalmente la caracteristica que se especifica en el argumento 
			glColor3f(0,0,0);				//Especifica el color blanco que se usara en el texto a imprimir en la siguiente linea 
			drawStrokeText("PRESIONE X PARA INICIAR",ancho*0.3,alto*0.40,0.2);	//llama a la funcion que dibuja el "string" y coloca en pantalla el texto.
			glEnable(GL_TEXTURE_2D);		//Habilita de nuevo la caracteristica "GL_TEXTURE_2D" que se habia deshabilitado unas lineas antes
			}
		
		//Impresión del nombre de usuario
		glDisable(GL_TEXTURE_2D);		//Apaga temporalmente la caracteristica que se especifica en el argumento. 
		glColor3f(0,0,1);				//Especifica el color azul que se usara en el texto a imprimir en la siguiente linea.
		drawStrokeText(nombre, ancho-250,10,0.2);	//Imprime el nombre que haya sido introducido por el usuario y que se guardo en la variable "nombre"
		glEnable(GL_TEXTURE_2D);		//Habilita de nuevo la caracteristica "GL_TEXTURE_2D" que se habia deshabilitado unas lineas antes.
		
		glBindTexture(GL_TEXTURE_2D, texName);		//Une a la textura texName llamada a una texturizacion en 2D
		glBegin(GL_QUADS);							//Permite especificar como se delimitan los vertices de una primitiva, usando un cuadrilatero en este caso
			glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);	//glTexCoord2f permite establecer los valores de las coordenadas actuales
			glTexCoord2f(0.0, 1.0); glVertex3f(0.0, alto, 0.0);	//si se supera el valor entre 0 y 1 la textura se dibuja mas de una vez en pantalla.
			glTexCoord2f(1.0, 1.0); glVertex3f(ancho, alto, 0.0);	//glVertex3f especifica donde se colocan los vertices de la imagen que es textura. 
			glTexCoord2f(1.0, 0.0); glVertex3f(ancho, 0.0, 0.0);
		glEnd();												//glEnd permite cerrar las especificaciones abiertas en glBegin().
		//Dibujar la bola
		glBindTexture(GL_TEXTURE_2D, texBola);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); glVertex3f(xBola, yBola, 0.2);
			glTexCoord2f(0.0, 1.0); glVertex3f(xBola, yBola + 20, 0.2);
			glTexCoord2f(1.0, 1.0); glVertex3f(xBola + 20, yBola + 20, 0.2);
			glTexCoord2f(1.0, 0.0); glVertex3f(xBola + 20, yBola, 0.2);
		glEnd();
		
		//Dibujar barra
		anchobarra=ancho*0.15625; //Se ajusta el ancho de la barra al 15% actual de la ventana del juego
		posxbar2=posxbar1+anchobarra;//Nueva posición en la que se dibujará el lado derecho de la barra, dependiendo del nuevo ancho
		glBindTexture(GL_TEXTURE_2D, texbarra);
		//glBindTexture(GL_TEXTURE_2D, texcora);
		glBegin(GL_QUAD_STRIP);//Función para dibujar un rectangulo a partir de cuatro coordenadas
			glTexCoord2f(0.0, 0.0); glVertex3f(posxbar1, posybar1, 0.3);//Esquina inferior izquierda
			glTexCoord2f(0.0, 1.0); glVertex3f(posxbar1, posybar2, 0.3);//Esquina superior izquierda
			glTexCoord2f(1.0, 1.0); glVertex3f(posxbar2, posybar1, 0.3);//Esquina superior derecha
			glTexCoord2f(1.0, 0.0); glVertex3f(posxbar2, posybar2, 0.3);//Esquina inferior derecha
		glEnd();
		
		//Dibujar corazones de las vidas
		glBindTexture(GL_TEXTURE_2D, texcora);
		int siguiente=0;//Variable para indicar al programa donde empezar a dibujar el siguiente corazón, empezando desde el lado derecho de la pantalla
		for (int i=0; i<vidas; i++){//Se crea un bucle que dibuja la cantidad de corazones que indique la variable vida
			glBegin(GL_QUADS);//Inicio de la creación del rectangulo que contiene al corazón a dibujar
				glTexCoord2f(0.0, 0.0); glVertex3f(ancho-50-siguiente, 10, 0.4);//Las coordenadas donde se ubicará el corazón a dibujar
				glTexCoord2f(0.0, 1.0); glVertex3f(ancho-50-siguiente, 40, 0.4);
				glTexCoord2f(1.0, 1.0); glVertex3f(ancho-siguiente, 40, 0.4);
				glTexCoord2f(1.0, 0.0); glVertex3f(ancho-siguiente, 10, 0.4);
			glEnd();
			siguiente=siguiente+40; //Se actualiza la nueva posición de inicio de dibujo del corazón
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
		glDisable(GL_TEXTURE_2D);//Se desactiva el modo 2D para utilizar texturas de opengl para poder cambiar ciertos parametros 
								//tales como el color con el que dibujar, en este caso para cambiar el color de
								//las letras que se escribirán a continuación
		glColor3f(1,1,1); //Nuevo color con el que dibujar, Blanco
		drawStrokeText("Presione enter para continuar",ancho-ancho*0.45,15,0);//Se muestra en pantalla al usuario
																			  //que tecla debe pulsar para salir del juego
																			  
		glEnable(GL_TEXTURE_2D);//Se habilita de nuevo el modo 2D para usar las texturas
		
		glutIdleFunc(mueveImagen);//Se actualizan las nuevas posiciones del recuadro que contiene la imagen
								  //que se muestra en pantalla al perder
		//Inicio de impresión de las imagenes que se mueven
		glBindTexture(GL_TEXTURE_2D, texGameover);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(posx, posy, 0.1);
		glTexCoord2f(0.0, 1.0); glVertex3f(posx, posy + altoImagenY, 0.1);
		glTexCoord2f(1.0, 1.0); glVertex3f(posx + anchoImagenX, posy + altoImagenY, 0.1);
		glTexCoord2f(1.0, 0.0); glVertex3f(posx + anchoImagenX, posy, 0.1);
		glEnd();
		
		//Impresión del fondo que se muestra junto a las imagenes con movimiento
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
		
		//Se desactiva de nuevo el modo 2D para texturas
		glDisable(GL_TEXTURE_2D);
		glColor3f(1,1,1);//Color con el que se muestran las letras
		drawStrokeText("PRESIONE ENTER PARA CONTINUAR",ancho-ancho*0.45,15,0);//Mensaje a mostrar al usuario para avanzar
																			  //y sus coordenadas
		//Se reactiva el modo 2D para texturas
		glEnable(GL_TEXTURE_2D);
		
		glutIdleFunc(mueveBloques);//Función que actualiza las posicione en las que se va a imprimir las imagenes a mostrar
		glBindTexture(GL_TEXTURE_2D, texGane);	
		int altoBloque = alto/5;//Alto del bloque dependiendo del tamaño de la ventana
		int margenY = 30;//Espacio entre bloques
		
		for (int j = 0; j < 5; j++){//Se dibujarán 5 bloques
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
		
		//Se carga el fondo de pantalla que se muestra en la pantalla de inicio del juego
		glBindTexture(GL_TEXTURE_2D, texinicio);
		//Se dibuja el fondo
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.0, alto, 0.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(ancho, alto, 0.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(ancho, 0.0, 0.0);
		glEnd();
		//Se desactiva el modo 2D para texturas de opengl
		glDisable(GL_TEXTURE_2D);
		glutIdleFunc(mueveletras);//Actualización de las nuevas posiciones de las letras que se muestran en pantalla
								  //solamente se varían las posiciones en el eje y
		if (posyL7 > alto){//Si la última linea de pixeles de las letras que se imprimen se encuentra en la última linea
						   // de arriba de la ventana, se colocarán las letras más abajo de lo que el usuario pueda ver
						   // y así emerjan del lado de abajo de la pantalla, esto para crear un efecto de bucle
			posyL1=0-40;
			posyL2=posyL1-80;
			posyL3=posyL2-40;
			posyL4=posyL3-40;
			posyL5=posyL4-40;
			posyL6=posyL5-40;
			posyL7=posyL6-60;}
			
		glColor3f(1,1,1);//Color con el que se escribirán las letras, todas las lineas tienen el mismo color
		
		//Inicio de la escritura de las letras
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
		drawStrokeText("ARM CORTEX-A7 900MHz",ancho*0.4,posyL6,0.1);
		
		glColor3f(1,1,1);
		drawStrokeText("PRESIONE ENTER PARA TERMINAR",ancho*0.24,posyL7,0.1);
		glEnable(GL_TEXTURE_2D); //Se reactiva el modo 2D para texturas por si otra parte del código lo necesita
		}	
		
	glFlush();//Indica al SO que se terminó de realizar cambios y vacía los buffers del sistema para no sobrecargarlo
	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();//Le indica al GPU de la computadora que haga los cambios efectivos, todo el tiempo durante el cual
					  // se realizan cambios en la pantalla, estos no se muestran directamente en la ventana del juego
					  // si no que se almacenan en un buffer, esto para evitar posibles errores en la muestra de los cambios
}

void reshape(int w, int h){//Función que permite cambiar el tamaño de la pantalla del juego en general
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);//Función que ajusta el rectángulo de pixeles de la pantalla
											   // es decir, donde se comenzará a crear la pantalla, desde
											   // la coordenada 0,0 hasta la w,h
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glOrtho(0.0, (GLfloat) w, 0, (GLfloat) h, -1, 1);
	//gluPerspective(60.0, (GLfloat) w / (GLfloat) h, 10, 1000);
	
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//glTranslatef(-400.0, -250.0, -650.0);
	//glRotatef(-20.0, 1, 0, 0);
	//glRotatef(-20.0, 0, 1, 0);
	
	ancho = w;//Actualización del nuevo ancho de pantalla
	alto = h;//Asignación del nuevo alto de la pantalla o ventana
}

void mueveBola(void)
{	
   //int xBolaant=xBola;
   if((puede==0)&&(vidas!=0)){//Si el usuario presionó la tecla x para poder empezar a jugar y además cuenta con alguna vida
	   xBola +=0;//No se actualiza la posición en x ni en y de la bola
	   yBola +=0;}
	   
   else{
	   xBola += deltaX;
	   yBola += deltaY;
	   }
   //int mitadbarra=(ancho*0.15625)/2;
   //posybar2=alto;
   //int altbarra=(posybar2-posybar1);
   
   //Comprobación de choque de la bola con las paredes del área de juego
   if (xBola >= (ancho - 20)) {deltaX *= -1;}
   if (yBola >= (alto - 20)) {deltaY *= -1;}
   if (xBola < 0) {deltaX *= -1;}
   if (yBola < 0) {//Si la bola tocó el borde inferior de la pantalla
	   deltaY *= -1; 
	   vidas=vidas-1; //Se resta una vida
	   puede=0; //Se deshabilita el movimiento de la bola y la barra
	   xBola = (ancho*0.5-ancho*0.01); //Se reubica la bola a su posición inicial
	   yBola = posybar2+5; 
	   posxbar1=(ancho*0.5-ancho*0.1);//Se reubica la barra a su posición inicial
	   }
   if (vidas==0){avanza=2;} //Indica que el usuario perdió
   if (bloquesRestantes==0){avanza=3;}//Indica que el usuario ganó
   
   //Comprobación de choque de la bola con la barra
   //if ((yBola == 80)&&(((xBola<(posxbar1+mitadbarra))&&(xBola>posxbar1))||((xBola>(posxbar2-mitadbarra))&&(xBola<posxbar2)))){
   /*if ((yBola <= 80)&&(((xBola<(posxbar1+mitadbarra))&&(xBola>posxbar1))||((xBola>(posxbar2-mitadbarra))&&(xBola<posxbar2)))){
	   deltaY *= -1; 
   }
   
   if ((yBola == 80)&&((xBola==posxbar2)||(xBola==posxbar1))){
	   deltaY *= -1; 
	   deltaX *= -1;
   }
   
   if ((yBola < 80)&&((xBola==posxbar1)||((xBola)==posxbar2))){
	   deltaX *= -1;
   }*/
   
   int centroX = xBola + 10;
   int centroY = yBola + 10;
   
   
   
   
   //Choques con la barra 2.0:
   int xBarra = posxbar1;
   int yBarra = posybar1;
   int anchoBarra = posxbar2 - posxbar1;
   int altoBarra = posybar2 - posybar1;
   
   //Comprobar colision de la bola con la barra (general)
	if ((centroX > (xBarra - radio)) && (centroX < (xBarra + anchoBarra + radio)) && 
		  (centroY > (yBarra - radio)) && (centroY < (yBarra + altoBarra + radio))) {
			  
	    
			  
		int distanciaLadoIzquierdo = xBarra - centroX;
		int distanciaLadoDerecho = centroX - (xBarra + anchoBarra);
		int distanciaLadoArriba = centroY - (yBarra + altoBarra);
		
		if ((distanciaLadoArriba >= 0) && (distanciaLadoArriba < radio)) {
			deltaY *= -1;
			yBola += deltaY; //Impulso extra para solucionar un bug donde la bola rebotaba dentro de la barra
			
			//Aqui irian los angulos random
			//calcular los deltas, con los signos adecuados
		}
		//Lado izq o derecho
		if (((deltaX > 0) && (distanciaLadoIzquierdo >= 0) && (distanciaLadoIzquierdo < radio)) ||
				((deltaX < 0) && (distanciaLadoDerecho >= 0) && (distanciaLadoDerecho < radio))) {
			deltaX *= -1;
		}
	}
	
	
	//Lado abajo
	//if ((deltaY > 0) && (distanciaLadoAbajo >= 0) && (distanciaLadoAbajo < radio)) {
	//	deltaY *= -1;
	//	huboColision = GL_TRUE;	
	//}
	//Lado arriba
   
   

   int anchoBloque = (ancho / 13) - 2*margen;
   int altoBloque = (alto / 20) - 2*margen;

   //Comprobación de choque de la barra con los bloques
   for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 5; j++) {
			
			if (!bloques[i][j]) {
				continue;
			}
			
			//int centroX = xBola + 10;
			//int centroY = yBola + 10;
			int xBloque = (ancho / 13) * i + margen;
			int yBloque = alto * j / 20 + (3 * alto) / 4 + margen;
			
			
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
   glutPostRedisplay();
   
}

void keyboard (unsigned char key, int x, int y)//Función que se encargará de detectar cual tecla se presionó durante el
{											   // del juego, dependiendo en que pantalla del juego se encuentre el juego
											   //ya sea la inicial, la de juego en sí, la de perdió, la de ganó, o la de 
											   //los créditos, permitirá o reconocerá unas letras u otras
	
	if(avanza==0){//Si el juego está en la pantalla de inicio
		if (((key >= 48) && (key <= 90)) || ((key >= 97) && (key <= 122)) || (key == 32)) {//Si se presionan las letras
																						   //del abecedario
			nombre[indiceNombre++] = key;//La tecla presionada se asigna a una posición del arreglo de strings
			nombre[indiceNombre] = '\0';}
	   
		else if (key == 8){//Si se presiona la tecla de borrado, se ubica en la posición anterior del arreglo y coloca
						   //un espacio
			nombre[--indiceNombre] = '\0';}
		else if (key == 13){//Si se presiona enter se avanza a la pantalla de juego y se deshabilita esta sección del código
			avanza=1;}
		glutPostRedisplay();}//Redibujar la pantalla
	
	if(avanza!=0){//Si se encuentra en la pantalla de juego
	   switch (key) {
		  case 'x':
			if(avanza==1){//Si se presiona x y además el juego efectivamente está en la pantalla de juego
				puede=1;//Le indica al sistema que permita el movimiento de la bola y de la barra
				glutIdleFunc(mueveBola);}
				break;
		  case 'd':
			if((avanza==1)&&(puede==1)){//Si presionó la tecla d y se encuentra en la pantalla de juego y además se había 
										//presionado la tecla x para empezar
				if(posxbar1<=(ancho-ancho*0.15625)){//Si no se ha llegado al borde derecho de la pantalla
					//Se actualizan la nuevas coordenadas donde se dibujará la barra, aumentando 10 pixeles,
					//así se habrá movido la barra hacia la derecha
					posxbar1=posxbar1+10;
					posxbar2=posxbar1+anchobarra;
					glutPostRedisplay();}}
					break;
		  case 'a':
			if((avanza==1)&&(puede==1)){//Si presionó la tecla d y se encuentra en la pantalla de juego y además se había 
										//presionado la tecla x para empezar
				if(posxbar1>0){//Si no se ha llegado al borde izquierdo de la pantalla
					//Se actualizan la nuevas coordenadas donde se dibujará la barra, disminuyendo en 10 pixeles la coordenada
					//derecha de la barra, así se habrá movido la barra hacia la izquierda
					posxbar2=posxbar2-10;
					posxbar1=posxbar2-anchobarra;
					glutPostRedisplay();}}
					break;
		  case 13:
		   if(avanza==4){//Si se presiona enter y se encuentra en la pantalla de los créditos, se sale del juego
			   exit(0);}
		   if((avanza==3)||(avanza==2)){//Si presiona enter y se encuentra en la pantalla de perdió o ganó, se mostrará
										//la pantalla de los créditos
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

int main(int argc, char** argv)//Función principal del juego
{
   glutInit(&argc, argv);//Inicializar una nueva ventana
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);//Inicialización de la ventana con opciones de trabajo normal,
															//además de permitir la combinación de colores y poder ubicar
															//objetos a diferentes profundidades
   gglutInitWindowSize(1366, 700);//Función que dá un tamaño inicial a la pantalla
   glutInitWindowPosition(0, 0);//Posición inicial en el monitor de la ventana a crear
   glutCreateWindow("Micronoid");//Se crea la pantalla y se le asigna un nombre, que se mostrará en la esquina
								 //superior izquierda
   init();//Se inicializa la ventana, asignandole diferentes caracteríticas, como por ejemplo el color del fondo de la 
		  //ventana, además, es aquí donde se crean las diferentes texturas que se utilizarán en el transcurso del juego
		  //a partir de ciertas imágenes.
   glutDisplayFunc(display);//Se ejecuta la función que imprime objetos dependiendo del avance del juego
   glutReshapeFunc(reshape);//Función que se ejecuta constantemente para comprobar si se cambió el tamaño de la ventana
							//y realizar el cambio
   glutKeyboardFunc(keyboard);//Función que verifica el estado del teclado, si se presionó o no alguna tecla
   glutMainLoop();//Bucle infinito a la función principal del juego
   return 0; 
}
