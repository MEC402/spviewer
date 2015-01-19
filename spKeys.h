
/*
 *  spKeys.h -
 *  this is the interface for key and event handlers. New Key and event handler needs to implement this class 
 *  it also serves as the main interaction between key/event handlers and main
 */

#ifndef _SPKEYS_H_
#define _SPKEYS_H_

#include <vector>
#include <osg/PositionAttitudeTransform>
#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>
#include "eventListener.h"

class spKeys : public eventListener, public osgGA::GUIEventHandler
{

public:
  spKeys();
  ~spKeys(){};
  virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
  virtual void accept(osgGA::GUIEventHandlerVisitor& v) {v.visit(*this);};
  virtual bool spHandle(const osgGA::GUIEventAdapter& ea){};
      
};

#endif
