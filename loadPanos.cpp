#include <math.h>
#include <iostream>
#include "loadPanos.h"

loadPanos::loadPanos() {
	curpano = -1;
}

loadPanos::loadPanos(std::vector<Panorama *> plist, osg::Group *l) {	
	curpano = -1;
	panos = plist;
	std::cerr << "P02 = " << panos[0]->getNumColumns() << std::endl;
	rightSphere = new osg::Switch();
	rightSphere->setDataVariance(osg::Object::DYNAMIC);
	leftSphere = (osg::Switch *)l;
        for (int i=0; i < panos.size(); i++)
        {
          rsphere.push_back(new osg::Group);
          lsphere.push_back(new osg::Group);
	  rightSphere->addChild(rsphere[i].get());
	  leftSphere->addChild(lsphere[i].get());
        }
	objNode = new osg::Switch();
	objNode->setDataVariance(osg::Object::DYNAMIC);
}



//create a part of sphere with texture
osg::ref_ptr<osg::Geode> createGeode(int row, int column, int numRows, int numCols, osg::ref_ptr<osg::Image>** images, int nodeMask) {
	//   std::cerr << "Create Geode Method " << std::endl;
    // Create Sphere
    osg::ref_ptr<osg::Geode> sphere = new osg::Geode;
    osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	texture->setDataVariance(osg::Object::DYNAMIC);
    texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
    texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
    texture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
    texture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
    osg::StateSet *state= sphere->getOrCreateStateSet();
    state->setTextureAttributeAndModes(0, texture.get(), osg::StateAttribute::ON);
    texture->setImage(images[row][column].get());
    sphere->setNodeMask(nodeMask);
    // Calculate Horizontal & Vertical Angles
    float horAngle = (360.0 / numCols);
    float verAngle = (180.0 / numRows);
    // Create ivSphere
    float lastart = ((float)row * verAngle); 
    float lastop  = ((float)row * verAngle + verAngle);
    float lostart = ((float)column * horAngle); 
    float lostop  = ((float)column * horAngle + horAngle);
    //--TODO:
    ivSphere *ltop1;
    
	if(row>11)
		ltop1 = new ivSphere(10000.0, lostart, lostop, lastart, lastop, 20);

	else if (row>9)
		ltop1 = new ivSphere(50000.0, lostart, lostop, lastart, lastop, 20);

	else if (row>7)
		ltop1 = new ivSphere(95000.0, lostart, lostop, lastart, lastop, 20);

	else
		ltop1 = new ivSphere(100000.0, lostart, lostop, lastart, lastop, 20);
		
	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	sphere->addDrawable(ltop1); 	

    return sphere;
}

void loadPanos::loadNextImage() {
 
  std::cerr <<"Load Next Panorama .... " << std::endl;
  int findex;
	if (getIndex()==-1) 
        {
		// Get the number of columns and rows from the Panorama
                findex = 0;
		setIndex(findex);
         } 
         else {
		findex= getIndex();
		setIndex((findex+1+panos.size())%panos.size());
                findex = getIndex();
	}
        switchImagePano(findex);
}
void loadPanos::loadPrevImage() {
 
  std::cerr <<"Load Prev Panorama .... " << std::endl;
  int findex;
	if (getIndex()==-1) 
        {
		// Get the number of columns and rows from the Panorama
                findex = panos.size() - 1;
		setIndex(findex);
         } 
         else {
		findex= getIndex();
		setIndex((findex-1+panos.size())%panos.size());
                findex = getIndex();
	}
        switchImagePano(findex);
}

void loadPanos::switchImagePano(int iindex) 
{
 
  std::cerr <<" Change Panorama .... " << std::endl;
  //if the pana was not created yet, create it
  if (lsphere[iindex]->getNumChildren() == 0)
  {
      std::cerr << " Building New Goemetry "<< std::endl;
      buildGeometry(iindex);
  }

  std::cerr << " switch pano only" << std::endl;
   int cols = panos[iindex]->getNumColumns();
   int rows = panos[iindex]->getNumRows();
   col = cols;
   row = rows;
   osg::PositionAttitudeTransform* pat= new osg::PositionAttitudeTransform;

        for (int i = 0; i < rows; i++) {
           for (int j = 0; j < cols; j++) {	    
	     pat=(osg::PositionAttitudeTransform*)lsphere[iindex]->getChild(j + i * cols);	   	          	
		pat->setAttitude(panos[iindex]->getQuat());
			}	
	}
			
    leftSphere->setSingleChildOn(iindex);
    rightSphere->setSingleChildOn(iindex);
}

void loadPanos::buildGeometry(int pindex)
{
// Get the number of columns and rows from the Panorama
	int cols = panos[pindex]->getNumColumns();
	int rows = panos[pindex]->getNumRows();
	col = cols;
	row = rows;
        std::cerr << "Cols = " << col << " Row = " << row << std::endl;
	//create the ivsphere's with texture
	// using lsphere[pindex] assume created
	//rsphere.push_back( new osg::Group);
	//using rsphere[pindex];
              
	osg::PositionAttitudeTransform* pat= new osg::PositionAttitudeTransform;
	osg::ref_ptr<osg::PositionAttitudeTransform>* leftrotate = new osg::ref_ptr<osg::PositionAttitudeTransform>[rows * cols];
        std::cerr << "Build Geodes" << std::endl;
	for (int i = 0; i < rows; i++) {
	   for (int j = 0; j < cols; j++) {	    
		leftrotate[j + i * cols] = new osg::PositionAttitudeTransform;
		leftrotate[j + i * cols]->addChild(createGeode( i, j, rows, cols, panos[pindex]->getLeftImages(), 0x01));
        std::cerr << "Add left geode " << pindex << std::endl;
		lsphere[pindex]->addChild(leftrotate[j + i * cols].get());
		//pat=(osg::PositionAttitudeTransform*)leftrotate[j + i * cols]->getChild();	   	          	
		pat=(osg::PositionAttitudeTransform*)lsphere[pindex]->getChild(j + i * cols);	   
		pat->setAttitude(panos[pindex]->getQuat());
        std::cerr << "Add right  geode " << pindex << std::endl;
		rsphere[pindex]->addChild( createGeode( i, j, rows, cols, panos[pindex]->getRightImages(), 0x02)); 
			}	
		}
        std::cerr << "Geodes built" << std::endl;
	
}
