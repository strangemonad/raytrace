/*
 *  viewer.cpp
 *  A4
 *
 *  Custom GTK GL drawing area widget. Will be used for display of interactive
 *  ray tracing results.
 *
 *  Created by Shawn Morel on 6/24/06.
 *  smorel - 20090740
 *  Copyright 2006.
 *
 */

#include <iostream>
#include <math.h>

#include "sceneNode.hpp"
#include "primitive.hpp"
#include "viewer.hpp"

#define BUTTON_NONE 0x0
#define BUTTON_1 0x1
#define BUTTON_2 0x2
#define BUTTON_3 0x4

Viewer::Viewer()
{
   mSceneRoot = NULL;
   
   resetOrientation();
   resetPosition();
   
   activeButtons = BUTTON_NONE;
   mMode = 'p';
   oldMouseX = oldMouseY = 0.0;
   
   Glib::RefPtr<Gdk::GL::Config> glconfig;
   
   // Ask for an OpenGL Setup with
   //  - red, green and blue component colour
   //  - a depth buffer to avoid things overlapping wrongly
   //  - double-buffered rendering to avoid tearing/flickering
   glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB |
                                      Gdk::GL::MODE_DEPTH |
                                      Gdk::GL::MODE_DOUBLE);
   if (glconfig.is_null()) {
      // If we can't get this configuration, die
      abort();
   }
   
   // Accept the configuration
   set_gl_capability(glconfig);
   
   // Register the fact that we want to receive these events
   add_events(Gdk::BUTTON1_MOTION_MASK    |
              Gdk::BUTTON2_MOTION_MASK    |
              Gdk::BUTTON3_MOTION_MASK    |
              Gdk::BUTTON_PRESS_MASK      | 
              Gdk::BUTTON_RELEASE_MASK    |
              Gdk::VISIBILITY_NOTIFY_MASK);
}

Viewer::~Viewer()
{
   // Nothing to do here right now.
}

void 
Viewer::invalidate()
{
   // Force a rerender
   GtkAllocation allocation = get_allocation();
   get_window()->invalidate_rect(Gdk::Rectangle(&allocation), false);
}

void 
Viewer::on_realize()
{
   // Do some OpenGL setup.
   // First, let the base class do whatever it needs to
   /*Gtk::GL::DrawingArea::on_realize();
   Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();
   if (!gldrawable) return;
   if (!gldrawable->gl_begin(get_gl_context())) return;
   
   
   // Set up for perspective drawing 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glViewport(0, 0, get_width(), get_height());
   gluPerspective(40.0, (GLfloat)get_width()/(GLfloat)get_height(), 0.1, 1000.0);
   
   glShadeModel(GL_SMOOTH);
   glClearColor( 0.0, 0.0, 0.0, 0.0 );
   glEnable(GL_NORMALIZE); // keep vectors normalized.
   
   cachedQuad = gluNewQuadric();
   
   Sphere::list = glGenLists(1);
   glNewList(Sphere::list, GL_COMPILE);
   gluSphere(cachedQuad, 1.0, 30, 20);
   glEndList();
   
   
   // Set up the light for the scene sinc we aren't changing it.
   GLfloat light0Pos[] = {1.0, 1.0, 1.0, 0.0};
   glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
   
   GLfloat light0Ambient[] = {1.0, 1.0, 1.0, 0.0};
   glLightfv(GL_LIGHT0, GL_AMBIENT, light0Ambient);
   
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0); // Defaults to white.
   
   
   gldrawable->gl_end();*/
}


bool 
Viewer::on_configure_event(GdkEventConfigure* event)
{
   /*Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();
   if (!gldrawable) return false;
   if (!gldrawable->gl_begin(get_gl_context())) return false;
   
   
   // Set up perspective projection, using current size and aspect
   // ratio of display
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glViewport(0, 0, event->width, event->height);
   gluPerspective(40.0, (GLfloat)event->width/(GLfloat)event->height, 0.1, 1000.0);
   
   // Reset to modelview matrix mode
   glMatrixMode(GL_MODELVIEW);
   gldrawable->gl_end();
   
   invalidate();*/
   return true;
}


bool 
Viewer::on_expose_event(GdkEventExpose* event)
{
   /*Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();
   if (!gldrawable) return false;
   if (!gldrawable->gl_begin(get_gl_context())) return false;
   
   if (mDrawCricle) {
      drawTrackballCircle();
   }
   
   // change to model view for drawing
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glPushMatrix();
   
   // Clear framebuffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   // This could be moved into a head joint node
   glTranslated(mModelTx, mModelTy, mModelTz);
   glRotated(mModelRx, 1, 0, 0);
   glRotated(mModelRy, 0, 1, 0);
   glRotated(mModelRz, 0, 0, 1);
   
   //renderScene(mSceneRoot);
   glBegin(GL_LINES);
   glColor3d(1.0, 0.0, 0.0);
   glVertex3d(0.0, 0.0, 0.0);
   glVertex3d(5.0, 0.0, 0.0);
   
   glColor3d(0.0, 1.0, 0.0);
   glVertex3d(0.0, 0.0, 0.0);
   glVertex3d(0.0, 5.0, 0.0);
   
   glColor3d(0.0, 0.0, 1.0);
   glVertex3d(0.0, 0.0, 0.0);
   glVertex3d(0.0, 0.0, 5.0);
   glEnd();
   
   mSceneRoot->walk_gl(false);
   
   glPopMatrix();
   
   
   // Swap the contents of the front and back buffers so we see what we
   // just drew. This should only be done if double buffering is enabled.
   gldrawable->swap_buffers();
   gldrawable->gl_end();*/
   return true;
}


//void 
//Viewer::drawTrackballCircle()
//{
   /*int current_width = get_width();
   int current_height = get_height();
   
   // Set up for orthogonal drawing to draw a circle on screen.
   // You'll want to make the rest of the function conditional on
   // whether or not we want to draw the circle this time around.
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();
   glOrtho(0.0, (float)current_width, 
           0.0, (float)current_height, -0.1, 0.1);
   
   // change to model view for drawing
   glMatrixMode(GL_MODELVIEW);
   
   // Reset modelview matrix
   glLoadIdentity();
   
   // draw a circle for use with the trackball 
   glDisable(GL_LIGHTING);
   glEnable(GL_LINE_SMOOTH);
   glColor3f(1.0, 1.0, 1.0);
   double radius = current_width < current_height ? 
      (float)current_width * 0.25 : (float)current_height * 0.25;
   glTranslated((float)current_width * 0.5, (float)current_height * 0.5, 0);
   glBegin(GL_LINE_LOOP);
   for(size_t i=0; i<40; ++i) {
      double cosine = radius * cos(i*2*M_PI/40);
      double sine = radius * sin(i*2*M_PI/40);
      glVertex2f(cosine, sine);
   }
   glEnd();
   glColor3f(0.0, 0.0, 0.0);
   glDisable(GL_LINE_SMOOTH);
   
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   
   glMatrixMode(GL_MODELVIEW);*/
//}




bool Viewer::on_button_press_event(GdkEventButton* event)
{
   switch (event->button) {
      case 1:
         activeButtons |= BUTTON_1;
         break;
      case 2:
         activeButtons |= BUTTON_2;
         break;
      case 3:
         activeButtons |= BUTTON_3;
         break;
      default:
         break;
   }
   
   oldMouseX = event->x;
   oldMouseY = event->y;
   return true;
}

bool Viewer::on_button_release_event(GdkEventButton* event)
{
   switch (event->button) {
      case 1:
         activeButtons &= ~BUTTON_1;
         break;
      case 2:
         activeButtons &= ~BUTTON_2;
         break;
      case 3:
         activeButtons &= ~BUTTON_3;
         break;
      default:
         break;
   }
   return true;
}

bool Viewer::on_motion_notify_event(GdkEventMotion* event)
{
   double deltaX = event->x - oldMouseX;   
   double deltaY = event->y - oldMouseY;
   
   /*
    * p - position & orientation
    *
    * j - joints
    */
   switch (mMode) {
      case 'p':
         if (activeButtons & BUTTON_1) {
            mModelTx += deltaX/8;
            mModelTy -= deltaY/8;
         }
         if (activeButtons & BUTTON_2) {
            mModelTz += deltaY/4;
            mModelTz = min(mModelTz, 0.0);
         }
         if (activeButtons & BUTTON_3) {
            
            
            mModelRy += deltaX/4;
            mModelRx += deltaY/4;
            // XXX fix this
         }
         break;
         
      case 'j':
         break;
         
      default:
         break;
   }
   
   oldMouseX = event->x;
   oldMouseY = event->y;
   
   this->invalidate();
   return true;
}
