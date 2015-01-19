/*
 *  eyeSeparation.h: an event listener class
 *  it is responsible for changing the eye seperation, togle the stereo mode, get vertical/horizontal split and changing to
 *  anaglyphic mode
 */

#ifndef _EYESEPARATION_H_
#define _EYESEPARATION_H_

#include "eventListener.h"
#include <osgViewer/Viewer>

class eyeSeparation : public eventListener
{
  
public: //0.001f
  eyeSeparation(osgViewer::Viewer* a): eventListener(){eyeDistance = 0.001f;viewer=a;};
  virtual bool spHandle(const osgGA::GUIEventAdapter& ea);
  // virtual void checkListeners();
  
private:
  double eyeDistance;
  osgViewer::Viewer* viewer;
 
  
};

#endif
