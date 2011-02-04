/*
 *  viewer.hpp
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


#ifndef VIEWER_HPP
#define VIEWER_HPP

#include <gtkmm.h>
#include <gtkglmm.h>
#include <GL/gl.h>
#include <GL/glu.h>

class SceneNode;

// The "main" OpenGL widget
class Viewer 
   : public Gtk::GL::DrawingArea 
{

private:
   SceneNode *mSceneRoot;
   
   char mMode;
   
   int activeButtons;
   double oldMouseX, oldMouseY;
   
   double mModelRx, mModelRy, mModelRz, mModelTx, mModelTy, mModelTz;
      
public:
   
   Viewer();
   virtual ~Viewer();
   
   // A useful function that forces this widget to rerender. If you
   // want to render a new frame, do not call on_expose_event
   // directly. Instead call this, which will cause an on_expose_event
   // call when the time is right.
   void invalidate();
   
   /*
    * p - position & orientation
    *
    * j - joints
    */
   void setMode(char newMode) { mMode = newMode;}
   
   void resetPosition(void) { 
      mModelTx = mModelTy = mModelTz = 0.0; 
   }
   void resetOrientation(void) {
      mModelRx = mModelRy = mModelRz = 0.0; 
   }
   
   void resetJoints(void){
   } // XXX implement.
   
   // XXX this will need to change
   void setScene(SceneNode* root) { mSceneRoot = root;}
   
protected:
      
   // Events we implement
   // Note that we could use gtkmm's "signals and slots" mechanism
   // instead, but for many classes there's a convenient member
   // function one just needs to define that'll be called with the
   // event.
   
   // Called when GL is first initialized
   virtual void on_realize();
   
   // Called when our window needs to be redrawn
   virtual bool on_expose_event(GdkEventExpose* event);
   
   // Called when the window is resized
   virtual bool on_configure_event(GdkEventConfigure* event);
   
   
   // Called when a mouse button is pressed
   virtual bool on_button_press_event(GdkEventButton* event);
   
   // Called when a mouse button is released
   virtual bool on_button_release_event(GdkEventButton* event);
   
   // Called when the mouse moves
   virtual bool on_motion_notify_event(GdkEventMotion* event);
};

#endif // VIEWER_HPP
