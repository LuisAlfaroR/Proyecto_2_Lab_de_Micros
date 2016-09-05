#include <iostream>
#include <GL/glew.h>
#include <string.h>
#include <SDL2/SDL.h>

using namespace std;

//****************************************************************************************************************
//display.h*******************************************************************************************************
//****************************************************************************************************************

#ifndef DISPLAY_H
#define DISPLAY_H

class Display
{
  public: 
      Display(int width, int height, const string& title);
      virtual ~Display();
      void clear(float r, float g, float b, float a);
      void Update();
      bool IsClosed();

  protected:
  private:
      Display(const Display& other) {}
      void operator=(const Display& other) {}
      SDL_Window* m_window;
      SDL_GLContext m_glContext;
      bool m_isClosed;
};

#endif // DISPLAY_H

//****************************************************************************************************************
//Display.cpp*****************************************************************************************************
//****************************************************************************************************************

Display::Display(int width, int height,const string& title)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
	
	m_window=SDL_CreateWindow(title.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_OPENGL);
	m_glContext=SDL_GL_CreateContext(m_window);
	GLenum status=glewInit();
	
	if(status !=GLEW_OK){
		cerr<<"Glew failed to Initialize!!"<<endl;
	}
	m_isClosed=false;
}

Display::~Display()
{
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();	

}

void Display::clear(float r, float g, float b, float a){
	glClearColor(r,g,b,a);
	glClear(GL_COLOR_BUFFER_BIT);
}

bool Display::IsClosed()
{
	return m_isClosed;
}

void Display::Update()
{
	SDL_GL_SwapWindow(m_window);
	SDL_Event e;
	
	while(SDL_PollEvent(&e)){
		if(e.type==SDL_QUIT)
			m_isClosed=true;
		}
}

#ifndef MESH_H
#define MESH_H

class Mesh
{
  public: 
      Mesh();
      void Draw();
      virtual ~Mesh();
     
  protected:
  private:
      Mesh(const Mesh& other) {}
      void operator=(const Mesh& other) {}
};

#endif // MESH_H

int main()
{
    Display display(800,600,"Hola mundo!");
    cout<<"Holis"<<endl;
    while(!display.IsClosed()){
		display.clear(0.0f,0.15f,0.3f,1.0f);
		display.Update();
		}
    return 0;
}
