/*
 * 
 * simple Pano Loader convenience class
 * 
 * 
 * 
 */
#ifndef LOAD_PANOS_H
#define LOAD_PANOS_H


#include <iostream>
#include <osg/Group>
#include <osg/Image>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osgGA/GUIEventHandler>
#include <osg/ShapeDrawable>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include "Panorama.h"
#include "ivSphere.h"


class loadPanos 
{
  
public:
  
  loadPanos();
  loadPanos(std::vector<Panorama *> plist, osg::Group *l);

  int getWidth(){return width;};
  int getHeight(){return height;};
  void loadNextImage();
  void loadPrevImage();
  void buildGeometry(int pindex);
  void switchImagePano(int iindex);
  int getIndex() { return curpano; }
  void setIndex(int a) { curpano = a; }
  osg::Group *getGroupNode() { return rightSphere;}
  // TODO: Make sure return type isn't Group *
  osg::Switch *getObjNode() { return objNode; }

private:
  int curpano;
  
  //the node for right sphere
  osg::Switch* rightSphere;

  //the node responsible for storing various data. i.e. maya objects
  osg::Switch* objNode;
  
  //the geode nodes for displaying pictures on left and right sphere
  std::vector<osg::ref_ptr<osg::Group>> lsphere;
  std::vector<osg::ref_ptr<osg::Group>> rsphere;
  
  //the group node that responsible for rotating pictures on left sphere
  osg::Switch* leftSphere;
  
  // Panoramas
  std::vector<Panorama*> panos;
  
  //number of rows and colums for tiled images for each eyes
  int col;
  int row;
  
  //the width and height of the screen
  unsigned int width, height;

protected:
  
};

#endif
