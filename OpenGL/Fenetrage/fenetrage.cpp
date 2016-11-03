#pragma region INCLUDES

#ifdef _MSC_VER
#pragma comment(lib, "opengl32.lib")
#include <windows.h>
#endif

#include<stdio.h>
#include<stdlib.h>

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

#pragma endregion

EsgiShader g_BasicShader;

#pragma region PROTOTYPES
void mouse(int bouton, int etat, int x, int y);      // fonction souris
void DrawSquare();
float ConvertCohord(int x0, int SizeLenght);
void vRappelMenuPrincipal(int i);
void vClavier(unsigned char key, int x, int y);
void DrawPoly();
void DrawFenetre();
void StopTrace();
#pragma endregion


int x0, y0, n;
int nSousmenu1, nSousmenu2, nMenuprincipal; // Numéros (identifiants) des menus
int nTue = 0;

int width = 800;
int height = 600;

bool b_TraceActivated = false;
bool b_FenetrageActivated = false;

float g_Color[] = {
  1.0f, 0.0f, 0.0f
};

float g_Triangle[] = {
  -0.8f, +0.8f,	1.0f, 0.0f,	0.0f,// vertex 0
  -0.8f, -0.8f,	0.0f, 1.0f,	0.0f,// vertex 1
  +0.8f, +0.8f, 0.0f, 0.0f,	1.0f,// vertex 2
  +0.8f, -0.8f, 1.0f, 1.0f,	0.0f// vertex 3
};

int PolySize;
int FenetreSize;
float g_Poly[500];
float g_Fenetre[500];


void update() 
{
  glutPostRedisplay();
}

void animate()
{
  glViewport(0, 0, width, height);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  //glColorMask(GL_TRUE, GL_FALSE, GL_TRUE, GL_TRUE);
  glClear(GL_COLOR_BUFFER_BIT);

  //DrawSquare();

  DrawPoly();
  DrawFenetre();
	
	glUseProgram(0);

	glutSwapBuffers();
}

void DrawSquare()
{
  auto program = g_BasicShader.GetProgram();
  
  glUseProgram(program);

  auto position_location = glGetAttribLocation(program, "a_Position");
  auto color_location = glGetAttribLocation(program, "a_Color");

  glVertexAttribPointer(color_location, 3, GL_FLOAT, GL_TRUE, 5 * sizeof(float), g_Triangle + 2);
  glEnableVertexAttribArray(color_location);

  glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), g_Triangle);
  glEnableVertexAttribArray(position_location);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glDisableVertexAttribArray(position_location);
  glDisableVertexAttribArray(color_location);
}

void DrawPoly()
{
  auto program = g_BasicShader.GetProgram();
  glUseProgram(program);

  auto position_location = glGetAttribLocation(program, "a_Position");
  auto color_location = glGetAttribLocation(program, "a_Color");

  glVertexAttribPointer(color_location, 3, GL_FLOAT, GL_TRUE, 5 * sizeof(float), g_Poly + 2);
  glEnableVertexAttribArray(color_location);

  glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), g_Poly);
  glEnableVertexAttribArray(position_location);

  glDrawArrays(GL_TRIANGLE_FAN, 0, PolySize);

  glDisableVertexAttribArray(position_location);
  glDisableVertexAttribArray(color_location);
}

void DrawFenetre()
{
  auto program = g_BasicShader.GetProgram();
  glUseProgram(program);

  auto position_location = glGetAttribLocation(program, "a_Position");
  auto color_location = glGetAttribLocation(program, "a_Color");

  glVertexAttribPointer(color_location, 3, GL_FLOAT, GL_TRUE, 5 * sizeof(float), g_Fenetre + 2);
  glEnableVertexAttribArray(color_location);

  glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), g_Fenetre);
  glEnableVertexAttribArray(position_location);

  glDrawArrays(GL_TRIANGLE_FAN, 0, FenetreSize);

  glDisableVertexAttribArray(position_location);
  glDisableVertexAttribArray(color_location);
}


int main(int argc, const char* argv[])
{
  glutInit(&argc, (char**)argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowPosition(200, 100);
  glutInitWindowSize(width, height);
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
  glutKeyboardFunc(vClavier);
  glutMouseFunc(mouse);

  g_BasicShader.LoadVertexShader("basic.vs");
  g_BasicShader.LoadFragmentShader("basic.fs");
  g_BasicShader.CreateProgram();

  glutIdleFunc(update);

  glutDisplayFunc(animate);

#pragma region Menu Creation
  // Gestion de menus
  nSousmenu1 = glutCreateMenu(vRappelMenuPrincipal);
  glutAddMenuEntry("Rouge", 101);
  glutAddMenuEntry("Vert", 102);
  glutAddMenuEntry("Bleu", 103);
  glutAddMenuEntry("Jaune", 104);
  nMenuprincipal = glutCreateMenu(vRappelMenuPrincipal);

  glutAddSubMenu("Couleurs", nSousmenu1);
  glutAddMenuEntry("Polygone à Découper", 1);
  glutAddMenuEntry("tracé fenêtre", 2);
  glutAddMenuEntry("fenêtrage", 3);
  glutAddMenuEntry("remplissage", 4);
  glutAddMenuEntry("clear poly", 5);
  glutAddMenuEntry("clear fenetre", 6);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

#pragma endregion


  glutMainLoop();
  g_BasicShader.DestroyProgram();

  return 1;
}

void vRappelMenuPrincipal(int i)
{
  StopTrace();
  switch (i)
  {
  case 101 :
    printf("Clic rouge\n");
    g_Color[0] = 1.0f; g_Color[1] = 0.0f; g_Color[2] = 0.0f;
    break;

  case 102:
    printf("Clic vert\n");
    g_Color[0] = 0.0f; g_Color[1] = 1.0f; g_Color[2] = 0.0f;
    break;

  case 103:
    printf("Clic bleu\n");
    g_Color[0] = 0.0f; g_Color[1] = 0.0f; g_Color[2] = 1.0f;
    break;

  case 104:
    printf("Clic jaune\n");
    g_Color[0] = 1.0f; g_Color[1] = 1.0f; g_Color[2] = 0.0f;
    break;

  case 1:
    printf("Poly\n");
    b_TraceActivated = true;

    break;

  case 2:
    printf("Trace\n");
    b_FenetrageActivated = true;
    break;

  case 3:
    printf("Fenetrage\n");
    break;

  case 4:
    printf("Remplissage\n");
    break;

  case 5:
    for (size_t i = 0; i <= PolySize; i++)
    {
      g_Poly[i] = 0;
    }
    PolySize = 0;
    break;

  case 6:
    for (size_t i = 0; i <= FenetreSize; i++)
    {
      g_Fenetre[i] = 0;
    }
    FenetreSize = 0;
    break;

  default:
    break;
  }
}

void vClavier(unsigned char key, int x, int y)
{
  switch (key)
  {
  default:
    break;
  }
}

void mouse(int button, int state, int x, int y)
{


  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    if (b_TraceActivated)
    {
      g_Poly[PolySize * 5] = ConvertCohord(x, 0);
      g_Poly[PolySize * 5 + 1] = ConvertCohord(y, 1);
      g_Poly[PolySize * 5 + 2] = g_Color[0];
      g_Poly[PolySize * 5 + 3] = g_Color[1];
      g_Poly[PolySize * 5 + 4] = g_Color[2];
      PolySize++;
    }


    if (b_FenetrageActivated)
    {
      g_Fenetre[FenetreSize * 5] = ConvertCohord(x, 0);
      g_Fenetre[FenetreSize * 5 + 1] = ConvertCohord(y, 1);
      g_Fenetre[FenetreSize * 5 + 2] = g_Color[0];
      g_Fenetre[FenetreSize * 5 + 3] = g_Color[1];
      g_Fenetre[FenetreSize * 5 + 4] = g_Color[2];
      FenetreSize++;
      //printf("%d\n", FenetreSize);
    }


  }

  if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
  {
    StopTrace();
  }


}

void StopTrace() 
{
  b_TraceActivated = false;
  b_FenetrageActivated = false;
}

float ConvertCohord(int x0, int SizeLenght)
{
  if (SizeLenght == 0)
  {
    SizeLenght = width;
    SizeLenght *= -1;
    
  }
  else if(SizeLenght == 1)
  {
    x0 *= -1;
    SizeLenght = height;
  }
  
  return ((float)x0 + SizeLenght / 2) / (abs(SizeLenght) / 2);
}

