
#ifndef PANORAMA_H_
#define PANORAMA_H_  
#include <iostream>
#include <osg/Quat>
#include <osg/Image>
#include <osg/Node>
#include "tiles.h"

class Panorama { 
  
public:
	Panorama(std::string id, std::string name, std::string left_dir, std::string right_dir, std::string aquat, std::string ageom);
	~Panorama();
	  
	std::string getID(){ return id; }
	std::string getName(){ return name; }
	std::string getLeftDir(){ return left_dir; }
	std::string getRightDir(){ return right_dir; }
	std::string getGeom(){ return geom; }
	osg::ref_ptr<osg::Node> getMyGeometry(){ return mygeometry; }
	double getCameraRotation(){ return cameraRotation;  }
	void setCameraRotation(double horRotation) { cameraRotation = horRotation; }
	
	// TODO: Implement function.	
	std::string getQuatToString(std::string q){ return q; }
	osg::Quat getQuat() { return myquat; }
	void setQuat(osg::Quat aq) { myquat = aq; }
	osg::ref_ptr<osg::Image>** getLeftImages(){ return leftImages; }
	osg::ref_ptr<osg::Image>** getRightImages(){ return rightImages; }
	int getNumColumns(){ return left->getNumColumns(); };
	int getNumRows(){ return left->getNumRows(); };
	// Panorama Values
	std::string id;
	std::string name;
	std::string left_dir;
	std::string right_dir;
	std::string geom;
	osg::ref_ptr<osg::Node> mygeometry;
	osg::Quat myquat;
	double cameraRotation;
	double rotatedLeftSphere;  // unused
	double verticalRotation; // unused
	Tiles* left;
	Tiles* right;
	osg::ref_ptr<osg::Image>** leftImages;
	osg::ref_ptr<osg::Image>** rightImages;
};

#endif
