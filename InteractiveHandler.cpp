
#include "InteractiveHandler.h"
#include <iostream>
bool InteractiveHandler::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa, osg::Object*, osg::NodeVisitor* nv)
{
    if (ea.getHandled()) return false;

    if (!_image) return false;

    switch(ea.getEventType())
    {
    
        case(osgGA::GUIEventAdapter::PUSH):
        case(osgGA::GUIEventAdapter::RELEASE):
        {
            osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
            int x=ea.getX();
            int y=ea.getY();
	int lowerx=width/4;
	int lowery=height/4;
	int upperx=3*width/4;
	int uppery=3*height/4;
	
	
	
	 if(y>lowery && y<uppery && x>lowerx && x<upperx){
	    x=x-lowerx;
	    y=y-lowery;
	    y=height/2-y;
    	 

                return _image->sendPointerEvent(x, y, ea.getButtonMask());
            }
            break;
        }
        case(osgGA::GUIEventAdapter::KEYDOWN):
        case(osgGA::GUIEventAdapter::KEYUP):
        {
            osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
            int x,y;
            bool sendKeyEvent = mousePosition(view, nv, ea, x, y);

            if (sendKeyEvent)
            {
                return _image->sendKeyEvent(ea.getKey(), ea.getEventType()==osgGA::GUIEventAdapter::KEYDOWN);
            }
            break;
	    
	    
        }
        case (osgGA::GUIEventAdapter::RESIZE):
        {
            if (_fullscreen && _camera.valid())
            {
                _camera->setViewport(0, 0, ea.getWindowWidth(), ea.getWindowHeight());

                resize(ea.getWindowWidth(), ea.getWindowHeight());
                return true;  
            }
            break;
        }

        default:
            return false;
    }
    return false;
} 
