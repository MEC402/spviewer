#include "eyeSeparation.h"
#include <iostream>

bool eyeSeparation::spHandle(const osgGA::GUIEventAdapter& ea) {
  
	switch(ea.getEventType()) {
           
		case osgGA::GUIEventAdapter::KEYDOWN:
  
		switch(ea.getKey()) {

			case '1':
		  
				if (osg::DisplaySettings::instance()->getStereo()) {
					eyeDistance += 0.001;
					osg::DisplaySettings::instance()->setEyeSeparation(eyeDistance);
				}
		  
				return true;
				break;
		
			case '2':
		  
				if (osg::DisplaySettings::instance()->getStereo()) {
					eyeDistance -= 0.001;
					osg::DisplaySettings::instance()->setEyeSeparation(eyeDistance);
				}
		  
				return true;
				break;
		
			case 'A':
				
				if (osg::DisplaySettings::instance()->getStereo()) {
					osg::DisplaySettings::instance()->setStereo(false);
					viewer->getCamera()->setCullMask(0x01);
		   
				} else {
					osg::DisplaySettings::instance()->setStereo(true);
					viewer->getCamera()->setCullMask(0xffffffff);
				}
		  
				return true;
				break;
		
			case 'v':
			  
				if (osg::DisplaySettings::instance()->getStereo()) 
					osg::DisplaySettings::instance()->setStereoMode(osg::DisplaySettings::VERTICAL_SPLIT);
			  
				return true;
				break;
			
			case 'h':
			  
				if (osg::DisplaySettings::instance()->getStereo()) 
					osg::DisplaySettings::instance()->setStereoMode(osg::DisplaySettings::HORIZONTAL_SPLIT);
			  
			  return true;
			  break;
			
			case 'b':
			  
				if (osg::DisplaySettings::instance()->getStereo())
					osg::DisplaySettings::instance()->setStereoMode(osg::DisplaySettings::ANAGLYPHIC);
			  
				return true;
				break;
		 }  
	}
	  
	return true;
	
}
