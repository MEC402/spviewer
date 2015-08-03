 
/*
 * keyHandler.h - handle key command presses
 */

#ifndef _KEY_HANDLER_H_
#define _KEY_HANDLER_H_
#include <string>
#include <sstream>
#include <fstream>
#include <osg/Billboard>
#include <osg/PositionAttitudeTransform>
#include <osgText/Text>
#include <osgViewer/Viewer>
#include <osgGA/CameraManipulator>
#include "loadPanos.h"
#include "eventListener.h"
#include <osg/Camera>
#include "sys/types.h"

/*#include "sys/sysinfo.h"*/

#define STEPSIZE 0.00128
#define EYESEP 0.0010

class keyHandler : public eventListener {

public:

    void rotateCamera(char direction);
	keyHandler();
	keyHandler(std::vector<Panorama *> plist, loadPanos *alp,
             osgGA::CameraManipulator *acm,
             osgViewer::Viewer *aviewer,
             osg::Group *aroot);
//  imageHandler(std::string d,osgViewer::Viewer *av,osgGA::CameraManipulator* cm, osg::Group* rt,osg::Group* _root, int* row,int* col, AwesomiumImage *a);
    virtual bool spHandle(const osgGA::GUIEventAdapter& ea);
//    void setIndex(int a) { myoindex = a; }
//    int getIndex() { return myoindex; } 
    void resetView();
	void resetViewWithHome();
  
private:
    std::string filename;
    long totalVirtualMem ;
    long totalPhysMem;
    int myoindex;
	// struct sysinfo memInfo;
    bool ref;
    int myindex;
    double vm, rss;
    osgViewer::Viewer *myviewer;
    osg::Group* root;
    double hor, ver;
    double tangent;
    float zupdate, xupdate;
    double prevang,aspect, verang, preverang;
    double degree,degreeVer,x,y,z,  fovy;
    osg::Group* rotate;
    osg::Switch* panos;
    osg::ref_ptr<osg::PositionAttitudeTransform>* refSphere;
    osg::Billboard* bb;
    osgText::Text* text;
    std::stringstream s;
    osg::Camera* camera;
    std::ofstream myfile;
    int row;
    double eyeDistance;
    int col;
    int ang;
    loadPanos * ai;
    float a,b,c;
    osg::Vec3d rotated, myAxis;
    osg::ref_ptr<osgGA::CameraManipulator> cm;
    osg::ref_ptr<osg::PositionAttitudeTransform> pat;
	std::vector<Panorama *>myplist;
	osg::Switch * objNobj;
	// for objNode off/on
	bool isOn = true;
	
};

#endif
