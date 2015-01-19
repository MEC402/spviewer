/*
 * Menu class contains a hud camera to display the menu and an osg group node
 * that will be added to root in main. This node is for display the file dynamically
 * 
 * 
 * 
 * 
 */



#ifndef MENU_H_
#define MENU_H_

#include <string>
#include <iostream>
#include <osg/Image>
#include <osg/Camera>
#include <osg/Texture2D>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osgGA/EventVisitor>
#include <osgGA/GUIEventHandler>
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include "AwesomiumImage.h"
#include "Panorama.h"
#include "InteractiveHandler.h"

#ifdef USE_AWESOMIUM_C_API

#include <Awesomium/awesomium_capi.h>
#include <Awesomium/KeyboardCodes.h>

#else

#include <Awesomium/STLHelpers.h>
#include <Awesomium/WebKeyboardCodes.h>
#include <Awesomium/WebKeyboardEvent.h>
#include <Awesomium/BitmapSurface.h>
#include <Awesomium/WebCore.h>
#include <Awesomium/WebView.h>
#include <Awesomium/JSObject.h>

#endif


class Menu{

private:
  //osg::PositionAttitudeTransform** rotate;
  osg::ref_ptr<osg::Camera> camera;
  osg::ref_ptr<osg::Group> node;
  osg::ref_ptr<osg::Group> rotate;
  osg::ref_ptr<AwesomiumImage> image;
#ifdef USE_AWESOMIUM_C_API
  awe_webcore_initialize_default();
#else
  WebCore* g_webCore;
#endif
  
public:
  Menu(std::string, osg::Group*, std::vector<Panorama*>, int, int*, int*, bool);
  void updateMenu();
  void shutDown();
  osg::Camera* getCamera();
  osg::Group* getGroupNode();
  osg::Group* getRotationNode(){return rotate.get();};
  osg::ref_ptr<AwesomiumImage> getImage(){return image;};
  
};
#endif  
 
