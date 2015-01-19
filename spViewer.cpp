
/*
 * spViewer.cpp
 *
 *  Build file for simple stereo viewer of stereo panospheres
 */

#include <iostream>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include "Menu.h"
#include "spKeys.h"
#include "AwesomiumImage.h"
#include "ivSphere.h"
#include "MenuHandler.h"
#include "imageHandler.h"
#include "eyeSeparation.h"
#include "Panorama.h"
#include "XmlReader.h"
#include <libxml++-2.6/libxml++/libxml++.h>
#include <stdlib.h>
using namespace xmlpp;


int main( int argc, char** argv )
{
  // Set String Locale
  std::locale::global(std::locale(""));
  
  Document* d;
  // use an ArgumentParser object to manage the program arguments.
  osg::ArgumentParser arguments(&argc, argv);
  

  // Create viewer
  osgViewer::Viewer viewer(arguments);
  
  
  // Controls
  Controls controls;
  std::string controlsDir = "";
  arguments.read("--controls", controlsDir);
  std::vector<double> rotateDegree;
  
  if (controlsDir.size() == 0){
    
    // Revert to Default Controls
    std::cout << "Using Default Controls" << std::endl;
    controls.setDefaultControls();
    
  }
  
  
  
  // Menu
  std::string menu;
  arguments.read("--menu", menu);
  
  try {
      
    char actualpath [300];
    char *ptr;
    ptr = realpath(menu.c_str(), actualpath);
    
    menu = "file://" + (std::string) ptr;
    
  } catch(...) {
    
    std::cout << "[FATAL ERROR]: File \"" << menu << "\" doesn't exist." << std::endl;
    return 1;
    
  }
  
  
  
  // XML Panorama File
  std::vector<Panorama*> panoramas;
  std::string xmlFileName;
  arguments.read("--xml", xmlFileName);
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
    return EXIT_FAILURE;
    
  }
  
  
  // Is Xinerama Enabled?
  bool xineramaEnabled = false;
  xineramaEnabled = arguments.read("-xin");
  

  
  
  
  
  //TODO
  std::cout << "Launching Viewer..." << std::endl;
  
  
  
  // Create and set Scene Lights
  osg::ref_ptr<osg::Light> light1 = new osg::Light;
    light1->setLightNum(1);
    light1->setAmbient(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
    light1->setDiffuse(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
    light1->setPosition(osg::Vec4(0.0f,0.0f,0.0f,0.0f));
  
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
  
  osg::ref_ptr<osg::LightSource> source1 = new osg::LightSource;
  osg::ref_ptr<osg::LightSource> source2 = new osg::LightSource;
  osg::ref_ptr<osg::LightSource> source3 = new osg::LightSource;
  osg::ref_ptr<osg::LightSource> source4 = new osg::LightSource;
  source1->setLight(light1);
  source2->setLight(light2);
  source3->setLight(light3);
  source4->setLight(light4);


  // Create Root Node
  osg::ref_ptr<osg::Group> root = new osg::Group;
  
  
  // This node is for rotating the left sphere only
  osg::ref_ptr<osg::Group> leftRotate = new osg::Switch;
    leftRotate->setDataVariance(osg::Object::DYNAMIC);
    
  // Add LeftRotate to Root
  root->addChild(leftRotate.get());

  
  
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
  
  viewer.setUpViewAcrossAllScreens();
  viewer.setSceneData(root.get());



	
	 
  //viewer.getCamera()->setViewMatrix(osg::Matrix::lookAt(osg::Vec3d(0, 0, 0), rotated, osg::Vec3d(0, 0, -1)));

  viewer.getCamera()->setProjectionMatrixAsPerspective(90, 1.6875, 1.0f,10000.0f);
  viewer.getCamera()->setCullMask(0x00000001);
  viewer.getCamera()->setCullMaskLeft(0x00000001);
  viewer.getCamera()->setCullMaskRight(0x00000002);
  viewer.getCamera()->setAllowEventFocus(false);
      Menu* c = new Menu(menu, leftRotate.get(), panoramas, 2, &row, &col, xineramaEnabled);
	  root->addChild(c->getGroupNode());
	  root->addChild(c->getCamera());
	  
	  
	MenuHandler* h= new MenuHandler(c);
	
	
	osg::ref_ptr<osgGA::CameraManipulator>cm = new osgGA::TrackballManipulator;
	//create a listener and event handler
        spKeys* mykeyui = new spKeys();
	eyeSeparation* es=new eyeSeparation(&viewer);
	
 	imageHandler* ih= new imageHandler(xmlFileName,&viewer,cm.get(),leftRotate.get(),root.get(),&row, &col, c->getImage());
	//ih->setVector(rotateDegree);
        mykeyui->addListener(es);
	mykeyui->addListener(ih);
	
	
	 //TODO

    
	
	
        viewer.addEventHandler(mykeyui);
	viewer.addEventHandler(new osgViewer::ScreenCaptureHandler(new osgViewer::ScreenCaptureHandler::WriteToFile("filename", "jpg")));
	viewer.addEventHandler(h);

        
	osg::Vec3 *avec;
	osg::Quat q = osg::Quat(0.0, osg::Vec3d(1,0,0),
			      0.0, osg::Vec3d(0,1,0),
				0.0, osg::Vec3d(0,0,1));
	avec = new osg::Vec3(0, 10.0, 0.0);
	osg::Vec3d rotated = q * *avec;
	
	osg::DisplaySettings::instance()->setEyeSeparation(0.001f);
	cm->setHomePosition(osg::Vec3(0.0f,0.0f,0.0f),rotated,osg::Vec3(0.0f,0.0f,-1.0f),false);
	viewer.setCameraManipulator(cm,true);
	
	
	viewer.realize();
	
	


    
	
    while ( !viewer.done() ){
	c->updateMenu();
	
	viewer.frame();
    }
}





