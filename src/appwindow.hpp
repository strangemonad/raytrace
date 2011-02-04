/*
 *  appwindow.hpp
 *  A4
 *
 *  Main UI for the interactive ray tracer.
 *
 *  Created by Shawn Morel on 6/24/06.
 *  smorel - 20090740
 *  Copyright 2006.
 *
 */

#ifndef APPWINDOW_HPP
#define APPWINDOW_HPP

#include <gtkmm.h>

class SceneNode;
class Viewer;

class AppWindow : public Gtk::Window {
public:
   AppWindow();
   //void setScene(SceneNode * root){mViewer.setScene(root);}
   
protected:
      
   // Handlers for various menu elements.
   void onMenuClickStub(void);
   
   // Handlers for various menu elements.
   void onModeChange(char mode) { /*mViewer.setMode(mode);*/}
   void onCircleChange(void) { 
   }
   
   void onZBuffChange(void) { 
   }
   
   void onCullFrontChange(void) { 
   }
   
   void onCullBackChange(void) { 
   }
   
   void onResetPosition(void) { 
      //mViewer.resetPosition();
      //mViewer.invalidate();
   }
   
   void onResetOrientation(void) { 
      //mViewer.resetOrientation();
      //mViewer.invalidate();
   }
   
   void onResetJoints(void) { 
      //mViewer.resetJoints();
      //mViewer.invalidate();
   }
   
   void onResetAll(void) { 
      //mViewer.resetPosition();
      //mViewer.resetOrientation();
      //mViewer.resetJoints();
      //mViewer.invalidate();
   }
private:
      
   // A "vertical box" which holds everything in our window
   Gtk::VBox mVbox;
   
   // The menubar, with all the menus at the top of the window
   Gtk::MenuBar mMenubar;
   
   // Each menu
   Gtk::Menu mMenuApplication;
   Gtk::Menu mMenuEdit;
   Gtk::Menu mMenuMode;
   Gtk::Menu mMenuOptions;
   
   Gtk::Menu mMenuPicking;
   
   // The main OpenGL area
   Viewer mViewer;
   
};

#endif // APPWINDOW_HPP
