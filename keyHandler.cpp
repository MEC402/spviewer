/*
 * keyHandler.cpp - implement key press handler code
 */

#include "keyHandler.h"
#include <osg/Camera>
#include <osg/Geometry>
#include <iostream>
#include <math.h>
#include <osg/Image>
#include <osgViewer/ViewerEventHandlers>
#include <osg/StateSet>
#include <osg/PolygonMode>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Texture2D>
#include "ivSphere.h"
#ifdef WIN32
#define _USE_MATH_DEFINES
#include <cmath>
#endif

#include <math.h>
#include <iostream>
#include "sys/types.h"
/*#include "sys/sysinfo.h"*/
#ifndef WIN32
#include <unistd.h>
#endif
#include <ios>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/* temporary global variables that need to be replaced */
/* will not work as advertised  these should be in the panoramas */
//static std::vector<double> rotatedLeftSphere;
//static std::vector<double> verticalRotation;
//static std::vector<osg::Quat> myQuat;
//static std::vector<std::string> Ids;

void process_mem_usage(double& vm_usage, double& resident_set)
{
   using std::ios_base;
   using std::ifstream;
   using std::string;

   vm_usage     = 0.0;
   resident_set = 0.0;

   // 'file' stat seems to give the most reliable results
   //
   ifstream stat_stream("/proc/self/stat",ios_base::in);

   // dummy vars for leading entries in stat that we don't care about
   //
   string pid, comm, state, ppid, pgrp, session, tty_nr;
   string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   string utime, stime, cutime, cstime, priority, nice;
   string O, itrealvalue, starttime;

   // the two fields we want
   //
   unsigned long vsize;
   long rss;

   stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
               >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
               >> utime >> stime >> cutime >> cstime >> priority >> nice
               >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

   stat_stream.close();
#ifndef WIN32
   long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
   vm_usage     = vsize / 1024.0;
   resident_set = rss * page_size_kb;
#endif
}
osg::ref_ptr<osg::Geode>  createRefSphere(int row, int column, int numCols, int numRows)
{
	//create sphere
	osg::ref_ptr<osg::Geode> sphere = new osg::Geode;
	osg::ref_ptr<osg::Texture2D> texture=new osg::Texture2D;
	texture->setDataVariance(osg::Object::DYNAMIC);
	texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
	texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP);
	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP);
	osg::StateSet *state= sphere->getOrCreateStateSet();
	state->setTextureAttributeAndModes(0, texture.get(), osg::StateAttribute::ON);
	state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	state->setMode(GL_BLEND,osg::StateAttribute::ON);
	
	osg::ref_ptr<osg::Image> image = osgDB::readRefImageFile("./assets/reference/ref_gridline.png");
	
	texture->setImage( image.get() );
	float horAngle = (360.0 / numCols);
	float verAngle = (180.0 / numRows);
	float lastart = ((float)row * verAngle); 
	float lastop  = ((float)row * verAngle + verAngle);
	float lostart = ((float)column * horAngle); 
	float lostop  = ((float)column * horAngle + horAngle);
        ivSphere *ltop1 = new ivSphere(900.0,lostart,lostop, lastart, lastop, 20);
        sphere->addDrawable(ltop1); 	
	

        return sphere;
}




//TODO
void keyHandler::rotateCamera(char direction){
    
   osg::Vec3 *avec;
  osg::Quat q;
  
  
  switch(direction){
    case 'u':
      
      ver += 1;
      q = osg::Quat(((ver / 180.0) * M_PI), osg::Vec3d(1, 0, 0), 0.0, osg::Vec3d(0, 1, 0), ((hor / 180.0) * M_PI), osg::Vec3d(0, 0, 1));  
      break;
      
      
    case 'd':
      
      ver -= 1;
      q = osg::Quat(((ver / 180.0) * M_PI), osg::Vec3d(1, 0, 0), 0.0, osg::Vec3d(0, 1, 0), ((hor / 180.0) * M_PI), osg::Vec3d(0, 0, 1));
      break;
      
      
    case 'l':
      
      hor -= 1;
      q = osg::Quat(((ver / 180.0) * M_PI), osg::Vec3d(1, 0, 0), 0.0, osg::Vec3d(0, 1, 0), ((hor / 180.0) * M_PI), osg::Vec3d(0, 0, 1));
      break;
      
      
    case 'r':
      
      hor += 1;
      q = osg::Quat(((ver / 180.0) * M_PI), osg::Vec3d(1, 0, 0), 0.0, osg::Vec3d(0, 1, 0), ((hor / 180.0) * M_PI), osg::Vec3d(0, 0, 1));
      break;
      
      
    default:
      
      
      break;
  }
   avec = new osg::Vec3(0,10,0.0);
   rotated = q * *avec+ osg::Vec3d(a,b,c);
   myAxis =q * osg::Vec3d(10,0.0,0.0) ;   
  cm->setHomePosition(osg::Vec3d(a, b, c), rotated, osg::Vec3d(0, 0, -1),false);
   myviewer->home();
}


/*
 * blank constructor that does nothing
 */

keyHandler::keyHandler()
{
}

keyHandler::keyHandler(std::vector<Panorama *> plist, loadPanos *alp,
                       osgGA::CameraManipulator *acm,
                       osgViewer::Viewer *aviewer,
                       osg::Group *aroot)
{
	myplist = plist;
	std::cerr << "Panos Size == " << myplist.size() << std::endl;
  std::cerr << "Panos Columns == " << myplist[0]->getNumColumns() << std::endl;
  ai = alp;
  cm = acm;
  myviewer = aviewer;

  a = b = c = x = 0.0;
  y = 10.0; z = 0.0;
  eyeDistance = 0.001f;
  t=0.0025;
  panos = (osg::Switch *)aroot;
  col = myplist[0]->getNumColumns();
  row = myplist[0]->getNumRows();

}

#ifdef _KEYHANDLER_CLEAN
keyHandler::keyHandler(std::string d,osgViewer::Viewer *av, osgGA::CameraManipulator* _cm,osg::Group* rt, osg::Group* _root, int* _row, int* _col,AwesomiumImage *awe){
    hor=0;
    filename=d;
    ai=awe;
    zupdate=0.0f;
    xupdate=0.0f;
    cm=_cm;
    ver=0;
    a=0.0;
    b=0.0;
    c=0.0;
    x=0.0;
    y=10.0;
    z=0.0;
    eyeDistance = 0.001f;
    row=_row;
    col=_col;
    degreeVer=0;
    degree=0.0;
    aspect=1.6875;
    root=_root;
    ref=false;
    t=0.0025;
    myviewer=av;
    fovy=90.0;
    rotate=new osg::Group;
    panos= (osg::Switch*) rt;
    rotate->setDataVariance(osg::Object::DYNAMIC);
    osg::Vec3 *avec;
    osg::Quat q = osg::Quat(0.0, osg::Vec3d(1,0,0),
			  0.0, osg::Vec3d(0,1,0),
			    0.0, osg::Vec3d(0,0,1));
    avec = new osg::Vec3(0, 10, 0.0);
    rotated = q * *avec;
    myAxis=osg::Vec3d(10.0,0.0,0.0);
    
    ang=0;
    verang=0;
    
    text= new osgText::Text;
    text->setCharacterSize(0.05);
    text->setAxisAlignment(osgText::TextBase::SCREEN);
    text->setDataVariance(osg::Object::DYNAMIC);
    text->setPosition(osg::Vec3(01.0f,-0.0f,0.0f));
    
    // hud camera
    camera = new osg::Camera;
    camera->setReferenceFrame( osg::Camera::ABSOLUTE_RF);
    camera->setClearMask( GL_DEPTH_BUFFER_BIT );
    camera->setRenderOrder( osg::Camera::POST_RENDER);
    camera->setAllowEventFocus( false );
    camera->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF );

    camera->setViewMatrixAsLookAt(osg::Vec3d(a, b, c),rotated, osg::Vec3d(0, 0, -1));

   
    osg::ref_ptr<osg::Geode> textGeode = new osg::Geode;
    textGeode->addDrawable(text);
    camera->addChild(textGeode.get());   
    root->addChild(camera);
    
    pat= new osg::PositionAttitudeTransform;
    refSphere= new osg::ref_ptr<osg::PositionAttitudeTransform>[(12)*(12)] ;
    for(int i=0;i<12;i++){
      for(int j=0;j<12;j++){
	refSphere[12*i+j]=new osg::PositionAttitudeTransform;
	refSphere[i*(12)+j]->setDataVariance(osg::Object::DYNAMIC);
	refSphere[(12)*i+j]->addChild(createRefSphere(i,j,12, 12).get());
	refSphere[(12)*i+j]->setNodeMask(0x0);
	root->addChild(refSphere[i*(12)+j]);
      }
    }
         


  
}

#endif
bool keyHandler::spHandle(const osgGA::GUIEventAdapter& ea){
 switch(ea.getEventType())
  {
    
     case osgGA::GUIEventAdapter::KEYDOWN:
	  switch(ea.getKey())
 	  {
	    
	    case 'l':
		 a=a+50;
		  x=rotated.x()+50;
		  y=rotated.y();
		  z=rotated.z();
		 rotated=  osg::Vec3d(x,y,z);
		 cm->setHomePosition(osg::Vec3(a,b,c),rotated,osg::Vec3(0.0f,0.0f,-1.0f),false);
		 myviewer->home();
		 break;
		 
	    case 'L':
		  a=a-50;
		  x=rotated.x()-50;
		  y=rotated.y();
		  z=rotated.z();
		 rotated=  osg::Vec3d(x,y,z);
		  cm->setHomePosition(osg::Vec3(a,b,c),rotated,osg::Vec3(0.0f,0.0f,-1.0f),false);
		  myviewer->home();
		  break;
		  
	    case 'k':
		 b=b+50;
		 x=rotated.x();
		  y=rotated.y()+50;
		  z=rotated.z();
		 rotated=  osg::Vec3d(x,y,z);
		 cm->setHomePosition(osg::Vec3(a,b,c),rotated,osg::Vec3(0.0f,0.0f,-1.0f),false);
		 myviewer->home();
		 break;
		 
	    case 'K':
		  b=b-1500;
		  x=rotated.x();
		  y=rotated.y()-50;
		  z=rotated.z();
		  rotated=  osg::Vec3d(x,y,z);
		  cm->setHomePosition(osg::Vec3(a,b,c),rotated,osg::Vec3(0.0f,0.0f,-1.0f),false);
		  myviewer->home();
		  break;
	
            //toggle the two pictures
	    case 't':
		if(myviewer->getCamera()->getCullMaskLeft()==0x01){
		  myviewer->getCamera()->setCullMaskLeft(0x00000002);
		  myviewer->getCamera()->setCullMaskRight(0x00000001);	  
		}
		else{
		  myviewer->getCamera()->setCullMaskLeft(0x00000001);
		  myviewer->getCamera()->setCullMaskRight(0x00000002);}
		return true;
		break;
		
	    //rotate left sphere vertically
	    case 'p':
		myindex=getIndex();
//                std::cerr << "Index " << myindex << std::endl;
		myplist[myindex]->rotatedLeftSphere-=t;
//                std::cerr << "Index 1 " << myindex << std::endl;
		degree=myplist[myindex]->rotatedLeftSphere;
//                std::cerr << "Index 2 " << myindex << std::endl;
		rotate= (osg::Group*)panos->getChild(myindex);

		
//                std::cerr << "Index 3" << row << " " << col << std::endl;
		    for (int i=0;i<(row)*(col); i++)
		    {			
//                std::cerr << "Pat 3" << myindex << std::endl;
			pat=(osg::PositionAttitudeTransform*)rotate->getChild(i);
//                std::cerr << "Pat 4" << pat << std::endl;
                        if (pat != NULL)
                        {
			   osg::Quat x=pat->getAttitude();		
			   x=x*osg::Quat(-t, osg::Vec3d(0.0f,0.0f,1.0f));
			   pat->setAttitude(x);
                        }
                        else
                        {
                std::cerr << "Pat == NULL " << pat << std::endl;
                        }
		    }
 		
//                std::cerr << "Index 4" << myindex << std::endl;
                  if (pat != NULL)
		    myplist[myindex]->myquat=pat->getAttitude();
		  return true;
		  break;

	    //rotate left sphere vertically
	    case 'P':
		 myindex=getIndex();
		 myplist[myindex]->rotatedLeftSphere+=t;
		 degree=myplist[myindex]->rotatedLeftSphere;
		 rotate= (osg::Group*)panos->getChild(myindex);

		    for (int i=0;i<(row)*(col); i++)
		    {
			pat=(osg::PositionAttitudeTransform*)rotate->getChild(i);
			osg::Quat x=pat->getAttitude();
			pat->setAttitude(x*osg::Quat(t, osg::Vec3d(0.0f,0.0f,1.0f)));
		    }
    		  myplist[myindex]->myquat=pat->getAttitude();

		  return true;
		  break;
		  
	    //load the next image
	    case 'n':
    
		  ai->loadNextImage();
		  myindex=getIndex();
		 // rotate= (osg::Group*)panos->getChild(myindex);
                  std::cerr << "Show Next Image ..." << std::endl;
	          return true;
		  break;
	    //load the previous image
	    case 'N':
		  ai->loadPrevImage();
  		  myindex=getIndex();
	          return true;
		  break;
	    
 	    //rotate left sphere horizontally
	    case 'o':
		// degreeVer=degreeVer+t;
		 myindex=getIndex();
		myplist[myindex]->verticalRotation-=t;
		degreeVer=myplist[myindex]->verticalRotation;
		rotate= (osg::Group*)panos->getChild(myindex);
		 for (int i=0;i<(row)*(col); i++)
		   {
			pat=(osg::PositionAttitudeTransform*)rotate->getChild(i);
    
			osg::Quat x=pat->getAttitude();
			x = x*osg::Quat(-t, myAxis);
			pat->setAttitude(x);
		   }
		   		  myplist[myindex]->myquat=pat->getAttitude();

		  
		 return true;
		 break;
		 
 	    //rotate left sphere horizontally
	    case 'O':
		myindex=getIndex();
		myplist[myindex]->verticalRotation+=t;
		degreeVer=myplist[myindex]->verticalRotation;
		rotate= (osg::Group*)panos->getChild(myindex);
		    for (int i=0;i<(row)*(col); i++)
		    {
			pat=(osg::PositionAttitudeTransform*)rotate->getChild(i);
    
			osg::Quat x=pat->getAttitude();
			x = x*osg::Quat(t, myAxis);
			pat->setAttitude(x);
		    }
		    		  myplist[myindex]->myquat=pat->getAttitude();

		  return true;
		  break;
	    
	    //increase field of view
	    case 'q':
		if(fovy>0)
		fovy-=1;
		
		myviewer->getCamera()->setProjectionMatrixAsPerspective(fovy, aspect, 1.0f,10000.0f);
		return true;
		break;
		
	    case '[':
		aspect+=0.1;

		myviewer->getCamera()->setProjectionMatrixAsPerspective(fovy, aspect, 1.0f,10000.0f);
		return true;
		break;
		
	    case ']':
		if(aspect>0.1)
		aspect-=0.1;
		
		myviewer->getCamera()->setProjectionMatrixAsPerspective(fovy, aspect, 1.0f,10000.0f);
		return true;
		break;
		
	    case 'z':
		if(fovy<180)
		fovy+=1;

		myviewer->getCamera()->setProjectionMatrixAsPerspective(fovy, aspect, 1.0f,10000.0f);
		return true;
		break;
		 
	    //load the reference grids
	    case 'R':
		    for(int i=0;i<(12);i++)
		      for(int j=0;j<(12);j++){
		  
			  if(refSphere[(12)*i+j]->getNodeMask()==0)
			    refSphere[(12)*i+j]->setNodeMask(0xffffffff);
			  else 
			    refSphere[(12)*i+j]->setNodeMask(0);
		    
			
		      }
		   
		  
		    break;
	    
	    //print out the degree of rotation
	     case 'g':
		     s.str("");
		     s<<"Degree of left sphere's rotation: \n"<<degree<<" "<<degreeVer;
		        s<<"\nEye seperation: \n"<<eyeDistance;
		     std::cout<<degree<<std::endl;
		     text->setText(s.str());
		   
		      myfile.open ("example.txt");
		      myfile << degree;
		      myfile.close();
		    break;
	    
	    //save the degree of rotation to file
	    case 'G':
		     s.str("");
		     text->setText(s.str());
//		     writeToFile(filename, Ids[myindex], std::to_string( degree ), std::to_string( degreeVer ));
		    break;
		 
	    case 'y':

		if(myviewer->getCamera()->getNodeMask() == 0x0){
		  myviewer->getCamera()->setNodeMask(0xffffffff);
		} else {
		  myviewer->getCamera()->setNodeMask(0X0);
		
		}
 		break;
	    case 'w':
		//rotateUp();
		rotateCamera('u');
		break;
	    case 's':
		//rotateDown();
		rotateCamera('d');
		break;
	     case 'a':
		rotateCamera('l');
		break;
	    case 'd':
		//rotateRight();
		rotateCamera('r');
		break;
	    case 'e':
	    
	      //reset the zoom
		  aspect=1.6875;
		  fovy=90.0;
		  myviewer->getCamera()->setProjectionMatrixAsPerspective(fovy, aspect, 1.0f,10000.0f);
		//reset the camera position view
		 hor = -1;
		 ver=0;
		  rotateCamera('u');
		 
		  rotated =osg::Quat(0.0, osg::Vec3d(1,0,0),
					  0.0, osg::Vec3d(0,1,0),
					    0.0, osg::Vec3d(0,0,1)) * osg::Vec3(0, 10, 0.0);
		
		   x=0	;
		  y=10;
		  z=0;
		 a=0.0;
		  b=0.0;
		  c=0.0;
		 cm->setHomePosition(osg::Vec3(0,0,0),osg::Vec3(0,10,0),osg::Vec3(0.0f,0.0f,-1.0f),false);
		 myviewer->home();
		degree=0.0;
		degreeVer=0.0;
		
		//reset the left sphere position
		for (int i=0;i<(row)*(col); i++)
		{
			pat=(osg::PositionAttitudeTransform*)rotate->getChild(i);
			pat->setAttitude(osg::Quat(degree, osg::Vec3d(0.0f,0.0f,1.0f)));
			pat->setAttitude(osg::Quat(degreeVer, osg::Vec3d(1.0f,0.0f,0.0f)));

		}
		break;
	    //increase eye Separation
	    case '1':
	  
		if (osg::DisplaySettings::instance()->getStereo()){
		  eyeDistance += 0.001;
		  osg::DisplaySettings::instance()->setEyeSeparation(eyeDistance);
	
		break;
	      
	      //decrease eye Separation
	      case '2':		
		if (osg::DisplaySettings::instance()->getStereo()){
		  eyeDistance -= 0.001;
		  osg::DisplaySettings::instance()->setEyeSeparation(eyeDistance);
		  
		}
		
		break;
	      
	      //set/unset stereo 
	      case '3':
		
		if (osg::DisplaySettings::instance()->getStereo()){
		  osg::DisplaySettings::instance()->setStereo(false);
		} else {
		  osg::DisplaySettings::instance()->setStereo(true);
		}
		
		break;
	      
	      //set vertical split
	      case 'v':
		
		if (osg::DisplaySettings::instance()->getStereo()){
		  osg::DisplaySettings::instance()->setStereoMode(osg::DisplaySettings::VERTICAL_SPLIT);
		  
		}
	
		break;
	     	//set horizonal split 
	      case 'h':
		
		if (osg::DisplaySettings::instance()->getStereo()){
		  osg::DisplaySettings::instance()->setStereoMode(osg::DisplaySettings::HORIZONTAL_SPLIT);
		}

		break;
	      
		//set ANAGLYPHIC mode
	      case 'b':
		
		if (osg::DisplaySettings::instance()->getStereo()){
		  osg::DisplaySettings::instance()->setStereoMode(osg::DisplaySettings::ANAGLYPHIC);
		}

		break;
	      
// 	      case 'A':
// 		if (osg::DisplaySettings::instance()->getStereo()){
// 		  osg::DisplaySettings::instance()->setStereo(false);
// 		  viewer->getCamera()->setCullMask(0x01);
// 		} else {
// 		  osg::DisplaySettings::instance()->setStereo(true);
// 		  viewer->getCamera()->setCullMask(0xffffffff);
// 		} 
	     case 'I':
    
		//double vm, rss;
		process_mem_usage(vm, rss);
		cout << "VM: " << vm << "; RSS: " << rss << endl;
#ifdef _MEMINFO_
		struct sysinfo memInfo;
		sysinfo (&memInfo);
		totalVirtualMem = memInfo.totalram;
		 //Add other values in next statement to avoid int overflow on right hand side...
		totalVirtualMem += memInfo.totalswap;
		totalVirtualMem *= memInfo.mem_unit;
		totalPhysMem = memInfo.totalram;
		//Multiply in next statement to avoid int overflow on right hand side...
		totalPhysMem *= memInfo.mem_unit;
		std::cout<<"RAM use: "<<totalPhysMem<<std::endl;
#endif
		break;

	  

	      return true;
	      default: return false;
		
   }
   return false;
  }
}}
