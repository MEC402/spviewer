
#include "MenuHandler.h"

MenuHandler::MenuHandler(Menu* m)
{
  menu = m;
  rotate=m->getRotationNode();
}


bool MenuHandler::handle( const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa )
{
  switch(ea.getEventType()){
//     case osgGA::GUIEventAdapter::DRAG:
//        return true;
//        break;
//      case osgGA::GUIEventAdapter::SCROLL:
//           switch (ea.getScrollingMotion())
//           {
//             case osgGA::GUIEventAdapter::SCROLL_UP:
//                      return true;
//                      break;
//             case osgGA::GUIEventAdapter::SCROLL_DOWN:
//                      return true;
//                      break;
//            }
           
    case(osgGA::GUIEventAdapter::KEYDOWN):
      
	if(ea.getKey() == 'm'){
	  
	  // Toggle Awesomium Menu Visibility
	  if(menu->getCamera()->getNodeMask() == 0x0){
	    menu->getCamera()->setNodeMask(0xffffffff);
	  } else {
	    menu->getCamera()->setNodeMask(0X0);	  
	  }
	}
	
	break;

     default:
       return false;
    
  }
  return false;
}