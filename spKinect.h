


#ifndef _SPKINECT_H_
#define _SPKINECT_H_

#include <vector>
#include <osg/PositionAttitudeTransform>
#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>
#include "eventListener.h"

class spKinect : public eventListener, public osgGA::GUIEventHandler
{

public:
	spKinect();
	~spKinect() {};
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	virtual void accept(osgGA::GUIEventHandlerVisitor& v) { v.visit(*this); };
	virtual bool spHandle(const osgGA::GUIEventAdapter& ea) { return true; };

};

#endif
