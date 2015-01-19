 
#ifndef _SCREENSHOT_H_
#define _SCREENSHOT_H_


#include <osgViewer/Viewer>
#include <osgViewer/ScreenCaptureHandler>
#include "eventListener.h"

class imageHandler : public eventListener, osgViewer::ScreenCaptureHandler
{

public:
  imageHandler(osgViewer::Viewer *av, osg::PositionAttitudeTransform** rt, osg::Group* _root);
  virtual bool spHandle(const osgGA::GUIEventAdapter& ea);
  // virtual void checkListeners();
  
private:
  osgViewer::Viewer *myviewer;
  osg::Group* root;
  double degree, t;
  osg::PositionAttitudeTransform** rotate;
  osg::Camera* camera;
  
};

#endif
