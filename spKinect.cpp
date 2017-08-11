
#include "spKinect.h"

spKinect::spKinect() : osgGA::GUIEventHandler() {}

bool spKinect::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {

	for (int i = 0; i < el.size(); i++) {
		el.at(i)->spHandle(ea);
	}

	return true;
}


