/*
 *  appwindow.cpp
 *  A4
 *
 *  Main UI for the interactive ray tracer.
 *
 *  Created by Shawn Morel on 6/24/06.
 *  smorel - 20090740
 *  Copyright 2006.
 *
 */

#include <iostream>

#include "viewer.hpp"
#include "appwindow.hpp"


AppWindow::AppWindow()
{
   set_title("Interactive Ray Tracer");
   
   using namespace Gtk::Menu_Helpers;
   
   
   // Set up the File menu.
   mMenuApplication.items().
      push_back(MenuElem("Reset Position",
                         Gtk::Menu_Helpers::AccelKey::AccelKey("I", ""),
                         SigC::slot(*this, &AppWindow::onResetPosition)));
   mMenuApplication.items().push_back(
                                      MenuElem("Reset Orientation",
                                               AccelKey::AccelKey("O", ""),
                                               SigC::slot(*this, &AppWindow::onResetOrientation)));
   mMenuApplication.items().push_back(
                                      MenuElem("Reset Joints",
                                               AccelKey::AccelKey("N", ""),
                                               SigC::slot(*this, &AppWindow::onResetJoints)));
   
   mMenuApplication.items().push_back(
                                      MenuElem("Reset All",
                                               AccelKey::AccelKey("A", ""),
                                               SigC::slot(*this, &AppWindow::onResetAll)));
   
   mMenuApplication.items().push_back(
                                      MenuElem("Quit",
                                               AccelKey::AccelKey("Q", ""),
                                               SigC::slot(*this, &AppWindow::hide)));
   
   // Set up the Mode menu.
   Gtk::RadioButton_Helpers::Group modes;
   mMenuMode.items().push_back(
                               RadioMenuElem::RadioMenuElem(modes,
                                                            "Position/Orientation",
                                                            Gtk::Menu_Helpers::AccelKey::AccelKey("P", ""),
                                                            SigC::slot(*this, &AppWindow::onMenuClickStub)));
   mMenuMode.items().push_back(
                               RadioMenuElem::RadioMenuElem(modes,
                                                            "Joints",
                                                            Gtk::Menu_Helpers::AccelKey::AccelKey("J", ""),
                                                            SigC::slot(*this, &AppWindow::onMenuClickStub)));
   
   // Set up the Edit menu.
   mMenuEdit.items().push_back(
                               MenuElem("Undo",
                                        AccelKey::AccelKey("U", ""),
                                        SigC::slot(*this, &AppWindow::onMenuClickStub)));
   mMenuEdit.items().push_back(
                               MenuElem("Redo",
                                        AccelKey::AccelKey("R", ""),
                                        SigC::slot(*this, &AppWindow::onMenuClickStub)));
   
   // Set up the Options menu.
   mMenuOptions.items().push_back(
                                  CheckMenuElem::CheckMenuElem("Circle",
                                                               Gtk::Menu_Helpers::AccelKey::AccelKey("C", ""),
                                                               SigC::slot(*this, &AppWindow::onCircleChange)));
   mMenuOptions.items().push_back(
                                  CheckMenuElem::CheckMenuElem("Z-buffer",
                                                               Gtk::Menu_Helpers::AccelKey::AccelKey("Z", ""),
                                                               SigC::slot(*this, &AppWindow::onZBuffChange)));
   mMenuOptions.items().push_back(
                                  CheckMenuElem::CheckMenuElem("Backface cull",
                                                               Gtk::Menu_Helpers::AccelKey::AccelKey("B", ""),
                                                               SigC::slot(*this, &AppWindow::onCullBackChange)));
   mMenuOptions.items().push_back(
                                  CheckMenuElem::CheckMenuElem("Frontface cull",
                                                               Gtk::Menu_Helpers::AccelKey::AccelKey("F", ""),
                                                               SigC::slot(*this, &AppWindow::onCullFrontChange)));
   
   // Set up the Picking menu.
   // Nothing for now.
   
   // Set up the menu bar.
   mMenubar.items().push_back(Gtk::Menu_Helpers::MenuElem("Application",
                                                          mMenuApplication));
   mMenubar.items().push_back(Gtk::Menu_Helpers::MenuElem("Edit",
                                                          mMenuEdit));
   mMenubar.items().push_back(Gtk::Menu_Helpers::MenuElem("Mode",
                                                          mMenuMode));
   mMenubar.items().push_back(Gtk::Menu_Helpers::MenuElem("Options",
                                                          mMenuOptions));
   
   mMenubar.items().push_back(Gtk::Menu_Helpers::MenuElem("Picking",
                                                          mMenuPicking));
   
   // Pack in our widgets.
   // First add the vertical box as our single "top" widget.
   add(mVbox);
   // Put the menubar on the top, and make it as small as possible
   mVbox.pack_start(mMenubar, Gtk::PACK_SHRINK);
   // Put the viewer below the menubar. pack_start "grows" the widget
   // by default, so it'll take up the rest of the window.
   mViewer.set_size_request(300, 600);
   mVbox.pack_start(mViewer);
   show_all();
}

void
AppWindow::onMenuClickStub(void)
{
   std::cerr << "Stub: clicked on a menu" << std::endl;
}
