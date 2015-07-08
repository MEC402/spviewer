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
    //--TODO
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

  if (getIndex()==-1) {
        std::cerr <<"Construct Geode .... " << std::endl;
		// Get the number of columns and rows from the Panorama
        std::cerr <<"Size .... " << panos.size() << std::endl;
		int cols = panos[0]->getNumColumns();
        std::cerr <<"Assign rows and columns .... " << std::endl;
		int rows = panos[0]->getNumRows();
        std::cerr <<"Assign rows and columns .... " << std::endl;
		col = cols;
		row = rows;
        std::cerr <<"Create ivSpheres/Group .... " << std::endl;
		//create the ivsphere's with texture
		lsphere.push_back( new osg::Group);
		rsphere.push_back( new osg::Group);
		osg::PositionAttitudeTransform* pat= new osg::PositionAttitudeTransform;
		osg::ref_ptr<osg::PositionAttitudeTransform>* leftrotate = new osg::ref_ptr<osg::PositionAttitudeTransform>[rows * cols];
        std::cerr <<"Build each geode .... " << std::endl;
		
		for (int i = 0; i < rows; i++) {
			
			for (int j = 0; j < cols; j++) {	    
				leftrotate[j + i * cols] = new osg::PositionAttitudeTransform;
				leftrotate[j + i * cols]->addChild(createGeode(i, j, rows, cols, panos[0]->getLeftImages(), 0x01));
				lsphere[0]->addChild(leftrotate[j + i * cols].get());
   				//pat=(osg::PositionAttitudeTransform*)leftrotate[j + i * cols]->getChild();	   	          	
   				pat=(osg::PositionAttitudeTransform*)lsphere[0]->getChild(j + i * cols);	   
				pat->setAttitude(panos[0]->getQuat());
				rsphere[0]->addChild( createGeode(i, j, rows, cols, panos[0]->getRightImages(), 0x02)); 
			}	
		}

		std::cerr <<"Add Children right.... " << std::endl;
		rightSphere->addChild(rsphere[0].get());
		std::cerr <<"Add Children Left.... " << std::endl;
		leftSphere->addChild(lsphere[0].get());
		std::cerr <<"Set Index.... " << std::endl;
		setIndex(0);
		std::cerr <<"Done.... " << std::endl;
		
		return;
      
    } else {
		int a= getIndex();
		setIndex((a+1)%panos.size());
		a= getIndex();
	
		//if the pana was not created yet, create it
		if (a>=lsphere.size()) {
			int cols = panos[a]->getNumColumns();
			int rows = panos[a]->getNumRows();
			col = cols;
			row = rows;
			//create the ivsphere's with texture
			lsphere.push_back( new osg::Group);
			rsphere.push_back( new osg::Group);
			osg::PositionAttitudeTransform* pat= new osg::PositionAttitudeTransform;
			osg::ref_ptr<osg::PositionAttitudeTransform>* leftrotate = new osg::ref_ptr<osg::PositionAttitudeTransform>[rows * cols];

			for (int i = 0; i < rows; i++) {
		
				for (int j = 0; j < cols; j++) {	    
					leftrotate[j + i * cols] = new osg::PositionAttitudeTransform;
					leftrotate[j + i * cols]->addChild(createGeode( i, j, rows, cols, panos[a]->getLeftImages(), 0x01));
					lsphere[a]->addChild(leftrotate[j + i * cols]);
					pat=(osg::PositionAttitudeTransform*)leftrotate[j + i * cols];	   	          	
					rsphere[a]->addChild( createGeode( i, j, rows, cols, panos[a]->getRightImages(), 0x02)); 
				}	
			}

		rightSphere->addChild(rsphere[a].get());
		leftSphere->addChild(lsphere[a].get());
		leftSphere->setSingleChildOn(a);
		rightSphere->setSingleChildOn(a);

		return;
		
		} else {
			int cols = panos[a]->getNumColumns();
			int rows = panos[a]->getNumRows();
			col = cols;
			row = rows;
  			osg::PositionAttitudeTransform* pat= new osg::PositionAttitudeTransform;

			for (int i = 0; i < rows; i++) {
	      
				for (int j = 0; j < cols; j++) {	    
					//leftrotate[j + i * cols] = new osg::PositionAttitudeTransform;
					//lsphere[a]->addChild(leftrotate[j + i * cols]);
					pat=(osg::PositionAttitudeTransform*)lsphere[a]->getChild(j + i * cols);	   	          	
					pat->setAttitude(panos[a]->getQuat());
				}	
			}
	      
			leftSphere->setSingleChildOn(a);
			rightSphere->setSingleChildOn(a);
		}
    }
}

void loadPanos::loadPrevImage() {
 
	if (getIndex()==-1) {
		// Get the number of columns and rows from the Panorama
		int cols = panos[0]->getNumColumns();
		int rows = panos[0]->getNumRows();
		col = cols;
		row = rows;
		//create the ivsphere's with texture
		lsphere.push_back( new osg::Group);
		rsphere.push_back( new osg::Group);
		osg::PositionAttitudeTransform* pat= new osg::PositionAttitudeTransform;
		osg::ref_ptr<osg::PositionAttitudeTransform>* leftrotate = new osg::ref_ptr<osg::PositionAttitudeTransform>[rows * cols];

		for (int i = 0; i < rows; i++) {
			
			for (int j = 0; j < cols; j++) {	    
				leftrotate[j + i * cols] = new osg::PositionAttitudeTransform;
				leftrotate[j + i * cols]->addChild(createGeode( i, j, rows, cols, panos[0]->getLeftImages(), 0x01));
				lsphere[0]->addChild(leftrotate[j + i * cols].get());
				//pat=(osg::PositionAttitudeTransform*)leftrotate[j + i * cols]->getChild();	   	          	
				pat=(osg::PositionAttitudeTransform*)lsphere[0]->getChild(j + i * cols);	   
				pat->setAttitude(panos[0]->getQuat());
				rsphere[0]->addChild( createGeode( i, j, rows, cols, panos[0]->getRightImages(), 0x02)); 
			}	
		}
	
		rightSphere->addChild(rsphere[0].get());
		leftSphere->addChild(lsphere[0].get());
		setIndex(0);
		
		return;
      
    } else {
		int a= getIndex();
		setIndex((a-1+lsphere.size())%panos.size());
		a= getIndex();
		//if the pana was not created yet, create it

		if (a>=lsphere.size()) {
			int cols = panos[a]->getNumColumns();
			int rows = panos[a]->getNumRows();
			col = cols;
			row = rows;
			//create the ivsphere's with texture
			lsphere.push_back( new osg::Group);
			rsphere.push_back( new osg::Group);
			osg::PositionAttitudeTransform* pat= new osg::PositionAttitudeTransform;
			osg::ref_ptr<osg::PositionAttitudeTransform>* leftrotate = new osg::ref_ptr<osg::PositionAttitudeTransform>[rows * cols];

			for (int i = 0; i < rows; i++) {
				
				for (int j = 0; j < cols; j++){	    
					leftrotate[j + i * cols] = new osg::PositionAttitudeTransform;
					leftrotate[j + i * cols]->addChild(createGeode( i, j, rows, cols, panos[a]->getLeftImages(), 0x01));
					lsphere[a]->addChild(leftrotate[j + i * cols]);
					pat=(osg::PositionAttitudeTransform*)leftrotate[j + i * cols];	   	          	
					pat->setAttitude(panos[a]->getQuat());
					rsphere[a]->addChild( createGeode( i, j, rows, cols, panos[a]->getRightImages(), 0x02)); 
				}	
			}
		  
			rightSphere->addChild(rsphere[a].get());
			leftSphere->addChild(lsphere[a].get());
			leftSphere->setSingleChildOn(a);
			rightSphere->setSingleChildOn(a);
	  
			return;
	  
		} else {
			int cols = panos[a]->getNumColumns();
			int rows = panos[a]->getNumRows();
			col = cols;
			row = rows;
			osg::PositionAttitudeTransform* pat= new osg::PositionAttitudeTransform;

			for (int i = 0; i < rows; i++) {
			
				for (int j = 0; j < cols; j++) {	    
					//leftrotate[j + i * cols] = new osg::PositionAttitudeTransform;
					//lsphere[a]->addChild(leftrotate[j + i * cols]);
					pat=(osg::PositionAttitudeTransform*)lsphere[a]->getChild(j + i * cols);	   	          	
					pat->setAttitude(panos[a]->getQuat());
				}	
			}
			
			leftSphere->setSingleChildOn(a);
			rightSphere->setSingleChildOn(a);
		}
	}
}
