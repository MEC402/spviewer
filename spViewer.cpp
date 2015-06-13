
/*
 * spViewer.cpp
 *
 *  Build file for simple stereo viewer of stereo panospheres
 */

#include <iostream>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include "eyeSeparation.h"
#include "spKeys.h"
#ifdef WIN32
#include "string.h"
#else
#include "strings.h"
#endif

// XML Parser
#include <mxml.h>
#include "panoXML.h"
#include "loadPanos.h"

// declare global panoram list
std::vector<Panorama*> Gpanoramas;

// code to handle key press events
#include "keyHandler.h"

void buildViewerScene(osgViewer::Viewer *aviewer,osg::Group *Groot, osgGA::CameraManipulator *Gcm, osg::Group *Gleftrotate);
void loadXMLfile(std::string xmlFileName);
spKeys* Gmykeyui = NULL;

/*
 * main function for startung up the panorama viewer
 *  The viewer has four major components
 *  An openSceneGraph Viewer 
 *  An openSceneGraph Scene
 *  An Awesomium HTML Menu object -- broken for most things
 *  An XMLParser using the libxml++ - conflicts on mac.
 */

int main(int argc, char** argv)
{
	bool hasXMLfile = false;
	std::string xmlFileName;
	std::cerr << "Spviewwer Started!" << std::endl;
	osg::ref_ptr<osg::Group> GleftRotate = new osg::Switch;
	std::cerr << "Group created" << std::endl;
#ifdef WIN32
	osg::ref_ptr<osgGA::CameraManipulator>Gcm = new osgGA::TrackballManipulator;
#else
	osg::ref_ptr<osgGA::CameraManipulator>Gcm = new osgGA::CameraManipulator;
#endif
	//osg::ref_ptr<osgGA::CameraManipulator>Gcm;
	std::cerr << "Camera Manipulator declared" << std::endl;
	osg::ref_ptr<osg::Group> Groot = new osg::Group;
   std::cerr << "Nodes Built!" << std::endl;

   // first we build our openscene graph viewer objec
   // we must have an openscene graph viewer object or there is no app.
  // use an ArgumentParser object to manage the program arguments.
  osg::ArgumentParser arguments(&argc, argv);
  hasXMLfile = arguments.read("--xml", xmlFileName);
//  hasXMLfile = arguments.read("--xml");
  osgViewer::Viewer viewer(arguments);
  std::cerr << "Viewer Built!" << std::endl;

  buildViewerScene(&viewer,Groot.get(),Gcm.get(),GleftRotate.get());
  std::cerr << "Scene Built!" << std::endl;


  if (hasXMLfile)
  {
    loadXMLfile(xmlFileName);
  }
  std::cerr << "XML Loaded" << std::endl;

  loadPanos *mylp = new loadPanos(Gpanoramas, GleftRotate.get());
  Groot->addChild(mylp->getGroupNode());
  std::cerr << "Panos Loaded" << std::endl;
  // install command key handler
// keyHandler* ih= new keyHandler(xmlFileName,&viewer,cm.get(),leftRotate.get(),root.get(),&row, &col, c->getImage());
  std::cerr << "P0 = " << Gpanoramas[0]->getNumColumns() << std::endl;
 keyHandler* ih= new keyHandler(Gpanoramas,mylp,Gcm.get(),&viewer, GleftRotate.get());
  Gmykeyui->addListener(ih);
  std::cerr << "Key Handler Added" << std::endl;

  // viewer.realize();
  // main loop to show and make viewer interactive
  while ( !viewer.done() )
  {
//	c->updateMenu();
    viewer.frame();
  }
}


void buildViewerScene(osgViewer::Viewer *aviewer, osg::Group *Groot, osgGA::CameraManipulator *Gcm, osg::Group *GleftRotate)
{
  //TODO
  std::cerr << "Building Viewer and Scene..." << std::endl;
  
  // Create and set Scene Lights
  osg::ref_ptr<osg::Light> light1 = new osg::Light;
  std::cerr << "Built light1..." << std::endl;
  light1->setLightNum(1);
    light1->setAmbient(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
    light1->setDiffuse(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
    light1->setPosition(osg::Vec4(0.0f,0.0f,0.0f,0.0f));
	std::cerr << "Set light1..." << std::endl;

  osg::ref_ptr<osg::Light> light2 = new osg::Light;
    light2->setLightNum(2);
    light2->setDiffuse(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
    light2->setPosition(osg::Vec4(0.0f,0.0f,0.0f,0.0f));
  
  osg::ref_ptr<osg::Light> light3 = new osg::Light;
    light3->setLightNum(3);
    light3->setDiffuse(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
    light3->setPosition(osg::Vec4(0.0f,0.0f,0.0f,0.0f));
  
  osg::ref_ptr<osg::Light> light4 = new osg::Light;
    light4->setLightNum(4);
    light4->setDiffuse(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
    light4->setPosition(osg::Vec4(0.0f,0.0f,0.0f,0.0f));
	std::cerr << "Lights Built!" << std::endl;

  osg::ref_ptr<osg::LightSource> source1 = new osg::LightSource;
  osg::ref_ptr<osg::LightSource> source2 = new osg::LightSource;
  osg::ref_ptr<osg::LightSource> source3 = new osg::LightSource;
  osg::ref_ptr<osg::LightSource> source4 = new osg::LightSource;
  source1->setLight(light1);
  source2->setLight(light2);
  source3->setLight(light3);
  source4->setLight(light4);
  std::cerr << "Light Sources Built!" << std::endl;

  // Create Root Node
  osg::Group * root = Groot;
  
  
  // This node is for rotating the left sphere only
   //GleftRotate = new osg::Switch();
   GleftRotate->setDataVariance(osg::Object::DYNAMIC);
    
  // Add LeftRotate to Root
 // root->addChild(GleftRotate.get());
  std::cerr << "Rotation Set!" << std::endl;

  int row = 0;
  int col = 0;
  
  root->getOrCreateStateSet()->setMode(GL_LIGHT1, osg::StateAttribute::ON);
  root->getOrCreateStateSet()->setMode(GL_LIGHT2, osg::StateAttribute::ON);
  root->getOrCreateStateSet()->setMode(GL_LIGHT3, osg::StateAttribute::ON);
  root->getOrCreateStateSet()->setMode(GL_LIGHT4, osg::StateAttribute::ON);
  root->addChild(source2.get());
  root->addChild(source3.get());
  root->addChild(source4.get());
  root->addChild(source1.get());  
  
  aviewer->setUpViewAcrossAllScreens();
  aviewer->setSceneData(root);
  std::cerr << "Scene Set!" << std::endl;

  //viewer.getCamera()->setViewMatrix(osg::Matrix::lookAt(osg::Vec3d(0, 0, 0), rotated, osg::Vec3d(0, 0, -1)));

  aviewer->getCamera()->setProjectionMatrixAsPerspective(90, 1.6875, 1.0f,10000.0f);
  aviewer->getCamera()->setCullMask(0x00000001);
  aviewer->getCamera()->setCullMaskLeft(0x00000001);
  aviewer->getCamera()->setCullMaskRight(0x00000002);
  aviewer->getCamera()->setAllowEventFocus(false);
  std::cerr << "Camera Properties Set!" << std::endl;

#ifndef WIN32
   Gcm = new osgGA::TrackballManipulator;
#endif
	//create a listener and event handler
        spKeys* mykeyui = new spKeys();
        Gmykeyui = mykeyui;
	eyeSeparation* es=new eyeSeparation(aviewer);
	
	//ih->setVector(rotateDegree);
        mykeyui->addListener(es);
	
        aviewer->addEventHandler(mykeyui);
//	aviewer->addEventHandler(new osgViewer::ScreenCaptureHandler(new osgViewer::ScreenCaptureHandler::WriteToFile("filename", "jpg")));
		std::cerr << "Handlers Set!" << std::endl;

        
	osg::Vec3 *avec;
	osg::Quat q = osg::Quat(0.0, osg::Vec3d(1,0,0),
			      0.0, osg::Vec3d(0,1,0),
				0.0, osg::Vec3d(0,0,1));
	avec = new osg::Vec3(0, 10.0, 0.0);
	osg::Vec3d rotated = q * *avec;
	
	osg::DisplaySettings::instance()->setEyeSeparation(0.001f);
	if (Gcm != NULL)
	{
		Gcm->setHomePosition(osg::Vec3(0.0f, 0.0f, 0.0f), rotated, osg::Vec3(0.0f, 0.0f, -1.0f), false);
	}
#ifdef WIN32
	
#else
	aviewer->setCameraManipulator(Gcm,true);
#endif
	
}
  
void loadXMLfile(std::string xmlFileName)
{
  std::vector<double> rotateDegree;
  std::cerr << "Parsing XML File..." << std::endl;

  FILE *fp;
  mxml_node_t *tree;

  fp = fopen(xmlFileName.c_str(), "r");
  if (fp == NULL) return;
  tree = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);
  fclose(fp);
  if (tree == NULL) return;

  Gpanoramas = parsePanos(tree);

//  mxml_node_t *tree = mxmlNewElement(MXML_NO_PARENT, "element");
#ifdef _SWITCH_OVER_PARSER
// XML Panorama File
  std::vector<Panorama*> panoramas;
   const xmlpp::Node* pNode;
  // Declare XMLReader
      XmlReader reader;
  // Read Panorama File
  //readXMLPanoramas(xmlFileName, panoramas);
  
  try {

    // Declare Parser
    xmlpp::DomParser parser;
    
    
    // Parser Settings
    //We can have the text resolved/unescaped automatically.
    parser.set_substitute_entities(true);
    
    
    // Parse through input XML file
    parser.parse_file(xmlFileName);
   
    
    if(parser){
      
      // Parse XML Tree
     pNode  = parser.get_document()->get_root_node();
    
     reader.setPnode(pNode);

      // Check XML File Type
      std::string result = reader.checkXMLFileType();

      
      // Panorama XML
      if(result.compare(reader.getXMLTYPEPANOS()) == 0){
	
	//TODO
	std::cout << "Loading Panoramas..." << std::endl;

	// Parse and Load Panoramas
	reader.parsePanoramas();

	// Get Panoramas
	panoramas = reader.getPanoramas();
	rotateDegree= reader.getDegree();
	//TODO
	std::cout << "Panoramas Loaded." << std::endl;

      // Other XML File
      } else {
	std::cout << "[ERROR] Unknown XML File: " << result << std::endl;
      }

    }
    
  } catch(const std::exception& ex) {
    
    std::cerr << "Exception caught: " << ex.what() << std::endl;
//    return EXIT_FAILURE;
    return ;
    
  }
#endif
}
