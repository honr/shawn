/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"

#ifdef ENABLE_VIS
#include "apps/vis/base/vis_liveview.h"
#include <stdio.h>
/** ------------------\ OPENGL /------------------ **/
#if defined(__APPLE_CC__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

namespace vis
{
GLuint textureID_[1];
//////////////////////////////////////////
/* forward declarations */
void initGL();
void reshape(int w, int h);
void display();
void idle();
void uploadTexture();
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void mouseMotion(int x, int y);
//////////////////////////////////////////
/** ------------------/ OPENGL \------------------ **/

#ifdef HAVE_BOOST
boost::mutex updateMutex_;
boost::thread *glThread;
#endif

bool updated_ = false;
bool created_ = false;
int resx_=0, resy_=0;
int sizex_=0, sizey_=0;
unsigned char *texture_ = NULL;

struct creator { 
   creator() { }
void operator()() 
{ 
   initGL();
} 
};

unsigned char* getTexture()
{
   return texture_;
}

#ifdef HAVE_BOOST
boost::mutex* getUpdateMutex()
{
   return &updateMutex_;
}
#endif


void createWindow(int sizex, int sizey, int resx, int resy)
{
   if(!created_)
   {
      created_ = true;
      resx_ = resx;
      resy_ = resy;
      sizex_ = sizex;
      sizey_ = sizey;
      texture_ = new unsigned char[resx_*resy_*4];
      for(int i=0; i<resx_*resy_*4; i++)
         texture_[i] = 255;
#ifdef HAVE_BOOST
      glThread = new boost::thread(creator());
#endif
   }
}

void updateTexture(unsigned char* textureData)
{
   if(updated_)
	   std::cout << "Idled Update" << std::endl;
   updated_ = true;
}

void initGL()
{
   int argc = 1;
   char** argv=new char*[2];
   argv[0] = "test";
   argv[1] = NULL;
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (sizex_, sizey_);
   glutInitWindowPosition (100, 100);
   glutCreateWindow("Vis::Liveview");
   glutReshapeFunc(reshape);
   glutDisplayFunc(display);
   glutIdleFunc(idle);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   glutMotionFunc(mouseMotion);

   glClearColor(0.3, 0.3, 0.3, 0.0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_TEXTURE_2D);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glGenTextures(1, textureID_);
   glBindTexture(GL_TEXTURE_2D, textureID_[0]);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, resx_, resy_, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, texture_);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NONE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NONE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

   glutMainLoop();
}

/**
 * Display() gets called whenever a new frame is to be rendered
 */
void display()
{
   if(updated_)
   {
#ifdef HAVE_BOOST
      boost::mutex::scoped_lock lock(updateMutex_);
#endif
	  std::cout << "Test 1" << std::endl;
      uploadTexture();
      updated_ = false;
	  std::cout << "Test 2" << std::endl;

   }

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();
   float midX = sizex_ * 0.5f;
   float midY = sizey_ * 0.5f;
   
   glBegin(GL_QUADS); 
   glColor3f(1.0f,1.0f,1.0f); 
	glTexCoord2f (0.0, 1.0);
	glVertex2f( midX-resx_*0.5f, midY-resy_*0.5f); // bottom left
	glTexCoord2f (1.0, 1.0);
	glVertex2f( midX+resx_*0.5f, midY-resy_*0.5f); // bottom right
	glTexCoord2f (1.0, 0.0);
	glVertex2f( midX+resx_*0.5f, midY+resy_*0.5f); // top right
	glTexCoord2f (0.0, 0.0);
   glVertex2f( midX-resx_*0.5f, midY+resy_*0.5f); // top left
   glEnd();

   // switch back and front buffer (the image is rendered to the back buffer, but the front buffer is displayed)
   glutSwapBuffers();
}

/**
 * idle() is called whenever there is nothing else to do
 */
void idle()
{
   

   glutPostRedisplay(); // render new frame
}

void uploadTexture()
{
#ifdef HAVE_BOOST
   //boost::mutex::scoped_lock lock(updateMutex_);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, resx_, resy_, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, texture_);
#endif
}

/**
 * keyboard is called whenever a key is pressed
 */
void keyboard(unsigned char key, int , int )
{
}

/**
 * mouse() is called whenever a mouse button is pressed or released
 */
void mouse(int button, int state, int x, int y)
{
}

/**
 * mouseMotion() is called whenever a button is pressed and the mouse is moved
 */
void mouseMotion(int x, int y)
{
}

/**
 * Reshape() is called whenever the window is resized
 */
void reshape(int w, int h)
{
  sizex_ = w;
  sizey_ = h;
  glViewport(0,0,(GLsizei)w,(GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D (0, sizex_, 0, sizey_);
  glMatrixMode(GL_MODELVIEW);
}

}
#endif