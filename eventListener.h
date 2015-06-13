 
/*
 *  eventListener.h - general event listener for spViewer
 *  This serves as the base class for all event listener. All the event listener need to implements this class
 *  It also acts as the way to interact with user. All the event listener need to be added to the eventListener vector of this class in main method
 *  
 */

#ifndef _EVENT_LISTENER_H_
#define _EVENT_LISTENER_H_
#include <vector>
#include <osgGA/GUIEventHandler>


class eventListener
{
public:
  
  eventListener(){};
  ~eventListener(){};
  
  virtual bool spHandle(const osgGA::GUIEventAdapter& ea){ return false; };
  virtual void addListener(eventListener *ael) { el.push_back(ael); } ;
    
  
protected:
  std::vector<eventListener*> el;
};

#endif
