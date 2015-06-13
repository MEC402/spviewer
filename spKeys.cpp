/*
 *  spKeys.cpp - define methods for spKeys class
 *  This class handles key and mouse events for the spViewer application.
 */

#include "spKeys.h"


spKeys::spKeys() : osgGA::GUIEventHandler(){}


bool spKeys::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
  
  for(int i = 0; i < el.size(); i++){
    el.at(i)->spHandle(ea);
  }
  return true;
}


