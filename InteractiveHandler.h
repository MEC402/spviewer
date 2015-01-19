/*
 * this class overrides the InteractiveImageHandler class
 * it is responsible for interacting between the menu and user
 * 
 * 
 * 
 * 
 */
#ifndef INTERACTIVE_HANDLER_H_
#define INTERACTIVE_HANDLER_H_
#include <osgViewer/ViewerEventHandlers>


class InteractiveHandler : public osgViewer::InteractiveImageHandler
{
  
public:
  
  // Constructor to use when the InteractiveImage is in the 3D scene (i.e. not in a fullscreen HUD overlay).
   InteractiveHandler(osg::Image* image, int x, int y): InteractiveImageHandler(image){width=x;height=y;};
  
  virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa, osg::Object*, osg::NodeVisitor* nv);


private:
  osg::Camera* camera;
  int x;
  int y;
  int width,height;
  osgViewer::View* view;
  bool sendKeyEvent ;
};
#endif