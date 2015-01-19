/*
 * The handler for showing on/off the menu
 * 
 * 
 */


#ifndef MENU_HANDLER_H_
#define MENU_HANDLER_H_ 
#include <osg/Camera>
#include <osgGA/GUIEventHandler>
#include "Menu.h"

class MenuHandler : public osgGA::GUIEventHandler{

public:
  MenuHandler(Menu* m);
  virtual bool handle( const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa );

private:
  Menu* menu;
  osg::Group* rotate;

};

#endif
