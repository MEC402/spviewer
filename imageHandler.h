 
/*
 * imageHandler: an event listener class
 * It is responsible for moving the camera left/right and back/forward, turning the camera up/down, and left/right, changing the field of view, zoom in/out
 * get the reference sphere, rotate left sphere and display/print out the degree of rotation
 */

#ifndef _IMAGE_HANDLER_H_
#define _IMAGE_HANDLER_H_
#include <string>
#include <sstream>
#include <fstream>
#include <osg/Billboard>
#include <osg/PositionAttitudeTransform>
#include <osgText/Text>
#include <osgViewer/Viewer>
#include <osgGA/CameraManipulator>
#include "AwesomiumImage.h"
#include "eventListener.h"
#include <osg/Camera>
#include "sys/types.h"
#include "sys/sysinfo.h"
#include "imageProcess.h"

class imageHandler : public eventListener, public imageProcess
{

public:

  void rotateCamera(char direction);
  imageHandler(std::string d,osgViewer::Viewer *av,osgGA::CameraManipulator* cm, osg::Group* rt,osg::Group* _root, int* row,int* col, AwesomiumImage *a);
  virtual bool spHandle(const osgGA::GUIEventAdapter& ea);
  
private:
  std::string filename;
  long totalVirtualMem ;
  long totalPhysMem;
  struct sysinfo memInfo;
    bool ref;
    int myindex;
    double vm, rss;
    osgViewer::Viewer *myviewer;
    osg::Group* root;
    int hor, ver;
    double tangent;
    float zupdate, xupdate;
    double prevang,aspect, verang, preverang;
    double degree,degreeVer,x,y,z, t, fovy;
    osg::Group* rotate;
    osg::Switch* panos;
    osg::ref_ptr<osg::PositionAttitudeTransform>* refSphere;
    osg::Billboard* bb;
    osgText::Text* text;
    std::stringstream s;
    osg::Camera* camera;
    std::ofstream myfile;
    int* row;
    double eyeDistance;
    int* col;
    int ang;
    AwesomiumImage * ai;
    float a,b,c;
      osg::Vec3d rotated, myAxis;
       osg::ref_ptr<osgGA::CameraManipulator> cm;
    osg::ref_ptr<osg::PositionAttitudeTransform> pat;

};

#endif
