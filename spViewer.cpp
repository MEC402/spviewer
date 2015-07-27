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


void buildViewerScene(osgViewer::Viewer *aviewer,osg::Group *Groot, osgGA::CameraManipulator *Gcm, osg::Group *Gleftrotate, bool multiscreen);
void loadXMLfile(std::string xmlFileName);
void printScreenInfo();
void setupViewScreens(osgViewer::Viewer *aviewer, double fovy, double aRatio, bool multiscreen);

spKeys* Gmykeyui = NULL;

/*
 * main function for startung up the panorama viewer
 *  The viewer has three major components
 *  An openSceneGraph Viewer 
 *  An openSceneGraph Scene
 *  An XMLParser using the xmxl library 
 */

int main(int argc, char** argv) {
	bool hasXMLfile = false;
	bool hasMultiScreen = false;
	std::string xmlFileName;
	std::cerr << "Spviewer Started!" << std::endl;
	osg::ref_ptr<osg::Group> GleftRotate = new osg::Switch;
	GleftRotate->setDataVariance(osg::Object::DYNAMIC);
	std::cerr << "Group created" << std::endl;
	osg::ref_ptr<osgGA::CameraManipulator>Gcm = new osgGA::TrackballManipulator;
	//osg::ref_ptr<osgGA::CameraManipulator>Gcm;
	std::cerr << "Camera Manipulator declared" << std::endl;
	osg::ref_ptr<osg::Group> Groot = new osg::Group;
	Groot->addChild(GleftRotate.get());
	std::cerr << "Nodes Built!" << std::endl;
    // first we build our openscene graph viewer object
    // we must have an openscene graph viewer object or there is no app.
    // use an ArgumentParser object to manage the program arguments.
    osg::ArgumentParser arguments(&argc, argv);
    hasXMLfile = arguments.read("--xml", xmlFileName);
    hasMultiScreen = arguments.read("--multiscreen");
    // hasXMLfile = arguments.read("--xml");
    if (hasXMLfile) {
	    loadXMLfile(xmlFileName);
     }

    osgViewer::Viewer viewer(arguments);
    std::cerr << "Viewer Built!" << std::endl;
    buildViewerScene(&viewer,Groot.get(),Gcm.get(),GleftRotate.get(),hasMultiScreen);
    std::cerr << "Scene Built!" << std::endl;

  
	std::cerr << "XML Loaded" << std::endl;

	if (hasXMLfile) {

		loadPanos *mylp = new loadPanos(Gpanoramas, GleftRotate.get());
		Groot->addChild(mylp->getGroupNode());


		// Add obj Node to Groot to handle geometry data and maya objects
		// room for future additions
		Groot->addChild(mylp->getObjNode());



		//if (Gpanoramas[0]->mygeometry != NULL) Groot->addChild(Gpanoramas[0]->mygeometry);
		std::cerr << "Panos Loaded" << std::endl;
		std::cerr << "P0 = " << Gpanoramas[0]->getNumColumns() << std::endl;
		keyHandler* ih = new keyHandler(Gpanoramas, mylp, Gcm.get(), &viewer, GleftRotate.get());
		// TODO: this is a test. Get rid of
		//ih->setIndex(0);
		//std::cout << "\n" << "Dis shit: " << ih->getIndex();
		Gmykeyui->addListener(ih);
		std::cerr << "Key Handler Added" << std::endl;
	
	} else 
		std::cerr << "No Panos to Load" << std::endl;
  
	// viewer.realize();
	// main loop to show and make viewer interactive
	while ( !viewer.done() ) {
		// c->updateMenu();
		viewer.frame();
	}
} 

void buildViewerScene(osgViewer::Viewer *aviewer, osg::Group *Groot, osgGA::CameraManipulator *Gcm, osg::Group *GleftRotate, bool multiscreen) {
	// TODO
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
	// GleftRotate = new osg::Switch();
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
	printScreenInfo();
	aviewer->getCamera()->setProjectionMatrixAsPerspective(34.0, 1080.0/1920.0, 1.0f,10000.0f);
//	aviewer->setUpViewAcrossAllScreens();
        setupViewScreens(aviewer, 34.8093072, 1080.0/1920.0,multiscreen);
	aviewer->setSceneData(root);
	std::cerr << "Scene Set!" << std::endl;
	// viewer.getCamera()->setViewMatrix(osg::Matrix::lookAt(osg::Vec3d(0, 0, 0), rotated, osg::Vec3d(0, 0, -1)));
	aviewer->getCamera()->setCullMask(0x00000001);
	aviewer->getCamera()->setCullMaskLeft(0x00000001);
	aviewer->getCamera()->setCullMaskRight(0x00000002);
	aviewer->getCamera()->setAllowEventFocus(false);
	std::cerr << "Camera Properties Set!" << std::endl;

	// create a listener and event handler
    spKeys* mykeyui = new spKeys();
    Gmykeyui = mykeyui;
	//eyeSeparation* es=new eyeSeparation(aviewer);
	// ih->setVector(rotateDegree);
    //mykeyui->addListener(es);
    aviewer->addEventHandler(mykeyui);
	//aviewer->addEventHandler(new osgViewer::ScreenCaptureHandler(new osgViewer::ScreenCaptureHandler::WriteToFile("filename", "jpg")));
	std::cerr << "Handlers Set!" << std::endl;
	osg::Vec3 *avec;
	osg::Quat q = osg::Quat(0.0, osg::Vec3d(1,0,0), 0.0, osg::Vec3d(0,1,0), 0.0, osg::Vec3d(0,0,1));
	avec = new osg::Vec3(0, 10.0, 0.0);
	osg::Vec3d rotated = q * *avec;
	osg::DisplaySettings::instance()->setEyeSeparation(0.001f);
	
	if (Gcm != NULL) 
		Gcm->setHomePosition(osg::Vec3(0.0f, 0.0f, 0.0f), rotated, osg::Vec3(0.0f, 0.0f, -1.0f), false);
	
	aviewer->setCameraManipulator(Gcm,true);
}
  
void loadXMLfile(std::string xmlFileName) {
	//std::vector<double> rotateDegree;
	std::cerr << "Parsing XML File..." << std::endl;
	FILE *fp;
	mxml_node_t *tree;
	fp = fopen(xmlFileName.c_str(), "r");

	if (fp == NULL) return;

	tree = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);
	const char* rootElement = mxmlGetElement(tree);
/*	This is not needed but held just in case.
	char compare[] = "panoramas";
	if (strcmp(rootElement, compare) != 0) 
		tree = tree->child;
*/
	
	fclose(fp);
	if (tree == NULL) return;
	Gpanoramas = parsePanos(tree);
	std::cerr <<"Gpanoramas: " << Gpanoramas.size();
}

void printScreenInfo()
{
	osg::GraphicsContext::WindowingSystemInterface* wsi = osg::GraphicsContext::getWindowingSystemInterface();
	if (!wsi)
	{
		osg::notify(osg::NOTICE) << "View::setUpViewAcrossAllScreens() : Error, no WindowSystemInterface available, cannot create windows." << std::endl;
		return;
	}


	double fovy, aspectRatio, zNear, zFar;
	//_camera->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);

	//    double fovx = atan(tan(osg::DegreesToRadians(fovy*0.5)) * aspectRatio) * 2.0;

	unsigned int numScreens = wsi->getNumScreens();
	std::cerr << "Numscreens = " << numScreens << std::endl;
	for (unsigned int i = 0; i<numScreens; ++i)
	{
		unsigned int width, height;
		wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(i), width, height);
		std::cerr << "Wxh = " << width << "x" << height << std::endl;
	}
}

void setupViewScreens(osgViewer::Viewer *aviewer, double fovy, double aRatio, bool multiscreen)
{
osg::GraphicsContext::WindowingSystemInterface* wsi = osg::GraphicsContext::getWindowingSystemInterface();
    if (!wsi)
    {
        osg::notify(osg::NOTICE)<<"View::setUpViewScreens() : Error, no WindowSystemInterface available, cannot create windows."<<std::endl;
        return;
    }


    double zNear, zFar;
    //_camera->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);

   double fovx = atan(tan(osg::DegreesToRadians(fovy*0.5)) * aRatio) * 2.0;
//   double fovx = osg::DegreesToRadians(30.0);

   unsigned int numScreens = 1;
   double rotate_x = 0.0;
   std::cerr << "Numscreens = " << numScreens << std::endl;
   if (multiscreen)
   {
       numScreens = 5;
       rotate_x = - double(numScreens-1) * 0.5 * fovx;
   }
   unsigned int width, height;
     wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(), width, height);
     std::cerr << "Wxh = " << width << "x" << height << std::endl;

            osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
            traits->screenNum = -1;
            traits->x = 0;
            traits->y = 0;
            traits->width = width;
            traits->height = height;
            traits->windowDecoration = false;
            traits->doubleBuffer = true;
            //traits->quadBufferStereo = true;
            traits->sharedContext = 0;
            traits->overrideRedirect = true;

            osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());


            osgViewer::GraphicsWindow* gw = dynamic_cast<osgViewer::GraphicsWindow*>(gc.get());
            if (gw)
            {
                osg::notify(osg::NOTICE)<<"  GraphicsWindow has been created successfully."<<gw<<std::endl;
            }
            else
            {
                osg::notify(osg::NOTICE)<<"  GraphicsWindow has not been created successfully."<<std::endl;
            }

       for(unsigned int i=0; i<numScreens; ++i, rotate_x += fovx)
        {
          std::cerr << "Camera #" << i << " " << (width/numScreens)*i << std::endl;
            osg::ref_ptr<osg::Camera> camera = new osg::Camera;
            camera->setGraphicsContext(gc.get());
            int lcorn = (width/numScreens);
            std::cerr << "Lcorn " << lcorn << " " << lcorn * i << std::endl;
            camera->setViewport(new osg::Viewport(lcorn*i, 0, lcorn, height));
            aviewer->addSlave(camera.get(), osg::Matrixd(), osg::Matrixd::rotate( rotate_x, 0.0, 1.0, 0.0));

       }
       aviewer->assignSceneDataToCameras();
}

