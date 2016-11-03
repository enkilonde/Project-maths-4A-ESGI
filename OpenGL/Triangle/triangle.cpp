
#ifdef _MSC_VER
#pragma comment(lib, "opengl32.lib")
#include <windows.h>
#endif

#include<stdio.h>


#ifdef NO_GLEW
#include <GL/gl.h>
#include "glext.h"

PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = nullptr;
#else
#include "glew.h"
#ifdef _MSC_VER
#pragma comment(lib, "glew32.lib")
#endif
#endif

#include "freeglut.h"

#include "../common/EsgiShader.h"

EsgiShader g_BasicShader;

//Prototypes
void mouse(int bouton, int etat, int x, int y);      // fonction souris


int x0, y0, n;

float g_Triangle[] = {

  -0.8f, +0.8f,	1.0f, 0.0f,	0.0f,// vertex 0
  -0.8f, -0.8f,	0.0f, 1.0f,	0.0f,// vertex 1
  +0.8f, +0.8f, 0.0f, 0.0f,	1.0f,// vertex 2

  //-0.8f, -0.8f,	1.0f, 0.0f,	0.0f,// vertex 0
  //+0.8f, -0.8f,	0.0f, 1.0f,	0.0f,// vertex 1
  +0.8f, -0.8f, 0.0f, 0.0f,	1.0f// vertex 3


};

static const float g_TriangleColor[] = {

  1.0f, 0.0f,	0.0f,// vertex 0
  0.0f, 1.0f,	0.0f,// vertex 1
  0.0f, 0.0f,	1.0f// vertex 2
};

int width = 800;
int height = 600;

void update() 
{
  glutPostRedisplay();
}

void animate()
{
  int TimeSinceAppStartedInMs = glutGet(GLUT_ELAPSED_TIME);
  float TimeInSeconds = (float)TimeSinceAppStartedInMs / 1000.0f;

	glViewport(0, 0, width, height);
	glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
	//glColorMask(GL_TRUE, GL_FALSE, GL_TRUE, GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT);

  auto program = g_BasicShader.GetProgram();

	glUseProgram(program);
	
  auto position_location = glGetAttribLocation(program, "a_Position");
  auto color_location = glGetAttribLocation(program, "a_Color");
  auto time_location = glGetUniformLocation(program, "u_Time");

  glUniform1f(time_location, TimeInSeconds);

  glVertexAttribPointer(color_location, 3, GL_FLOAT, GL_TRUE, 5 * sizeof(float), g_Triangle + 2);
  glEnableVertexAttribArray(color_location);

	glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), g_Triangle);
	glEnableVertexAttribArray(position_location);



  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableVertexAttribArray(position_location);
  glDisableVertexAttribArray(color_location);
	
	glUseProgram(0);

	glutSwapBuffers();
}

int main(int argc, const char* argv[])
{


	// passe les parametres de la ligne de commande a glut
	glutInit(&argc, (char**)argv);
	// defini deux color buffers (un visible, un cache) RGBA
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);
	// positionne et dimensionne la fenetre
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(width, height);
	// creation de la fenetre ainsi que du contexte de rendu
	glutCreateWindow("Triangle ESGI");
#ifdef NO_GLEW
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)
							wglGetProcAddress("glVertexAttribPointer");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)
							wglGetProcAddress("glEnableVertexAttribArray");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)
							wglGetProcAddress("glDisableVertexAttribArray");
#else
	glewInit();
#endif


  //Appel de la sourie
  glutMouseFunc(mouse);

	g_BasicShader.LoadVertexShader("basic.vs");
	g_BasicShader.LoadFragmentShader("basic.fs");
	g_BasicShader.CreateProgram();

  glutIdleFunc(update);

	glutDisplayFunc(animate);
	glutMainLoop();

	g_BasicShader.DestroyProgram();

	return 1;
}


void mouse(int button, int state, int x, int y)
{
  int n0;
  // Si on appuie sur le bouton de gauche
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    switch (n)
    {
    case 0:
      n0 = 0;
      break;

    case 1:
      n0 = 2;
      break;

    case 2:
      n0 = 3;
      break;

    case 3:
      n0 = 1;
      break;
    }
    g_Triangle[n0 * 5] = ((float)x - width / 2) / (width / 2);
    g_Triangle[n0 * 5 + 1] = (height / 2 - (float)y) / (height/2);
    
    n++;
    if (n > 3) n = 0;

    //printf("X : %d, X' : %f, Y : %d, Y' = %f\n", x, g_Triangle[0], y, g_Triangle[1]);
  }

  if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
  {
    n--;
    if (n < 0) n = 3;

    switch (n)
    {
    case 0:
      n0 = 0;
      break;

    case 1:
      n0 = 2;
      break;

    case 2:
      n0 = 3;
      break;

    case 3:
      n0 = 1;
      break;
    }

    g_Triangle[n0 * 5] = ((float)x - width / 2) / (width / 2);
    g_Triangle[n0 * 5 + 1] = (height / 2 - (float)y) / (height / 2);
    
    n++;
    if (n > 3) n = 0;

    //printf("X : %d, X' : %f, Y : %d, Y' = %f\n", x, g_Triangle[0], y, g_Triangle[1]);
  }
}
