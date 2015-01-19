
#include <iostream>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/Image>
#include <time.h>
#include "tiles.h"


// Tiles Constructors w/ Initalization Lists
Tiles::Tiles(std::string imageFile, int cutWidth, int cutHeight, bool debug = false)
  : imageFileName(imageFile)
  , debugFlag(debug)
  , scaleFlag(false)
  , cutWidth(cutWidth)
  , cutHeight(cutHeight)
  , xResolution(0)
  , yResolution(0)
  , calcWidth(0.0f)
  , calcHeight(0.0f)
  , totalTimeElapsed(0)
{
  
}

Tiles::Tiles(std::string imageFile, int cutWidth, int cutHeight, int xRes, int yRes, bool scale = false, bool debug = false)
  : imageFileName(imageFile)
  , debugFlag(debug)
  , scaleFlag(scale)
  , cutWidth(cutWidth)
  , cutHeight(cutHeight)
  , xResolution(xRes)
  , yResolution(yRes)
  , calcWidth(0.0f)
  , calcHeight(0.0f)
  , totalTimeElapsed(0)
{
 
}


int Tiles::getNumColumns(){
  return calcWidth;
  
}

int Tiles::getNumRows(){
  return calcHeight;
  
}

void Tiles::createTiles(){

  
  ////////////////////
  // Image Handling //
  ////////////////////
  
  // Read in a reference to the image
  osg::ref_ptr<osg::Image> image = osgDB::readRefImageFile(imageFileName);  
  
  
  // Scale original image if requested
  if(scaleFlag){
    std::cout << "Scaling image from (" << image->s() << "x" << image->t() << ") to (" << xResolution << "x" << yResolution << ")" << std::endl;
    image->scaleImage(xResolution, yResolution, 1);
  }
  
  // Get Image Dimensions
  panoWidth = image->s();
  panoHeight = image->t();
  
  // Calculate Image subsections (for offsets)
  calcWidth = (panoWidth / cutWidth);
  calcHeight = (panoHeight / cutHeight);
 
  
  if(debugFlag){
    std::cout << "Calculated Width:  " << calcWidth << std::endl;
    std::cout << "Calculated Height: " << calcHeight << std::endl;
  }
  
  
  
  //////////////////////
  // Create Subimages //
  //////////////////////
  
//   Declare Array to store Tiled Images
  tiledImages = new osg::ref_ptr<osg::Image>*[(int)(calcHeight)];
 
  
//   Initialize and Allocate Array
 for(int i = 0; i < (int)calcHeight; i++){
   tiledImages[i] = new osg::ref_ptr<osg::Image>[(int)calcWidth];
 }
  
  // Create sub-images and store them into tiledImages
  for( int i= (int)calcWidth-1;i>=0; i--){
    for( int j = 0; j<(int)calcHeight; j++){
      
      // Create Temporary sub-image
      osg::ref_ptr<osg::Image> tempImage = new osg::Image;

      // Cropping Values
      double source_minx = cutWidth * i;
      double source_miny = cutHeight * j;
      double source_maxx = cutWidth * (1 + i);
      double source_maxy = cutHeight * (1 + j);
      double temp_minx = 0;
      double temp_miny = 0;
      double temp_maxx = cutWidth;
      double temp_maxy = cutHeight;
      
 
      // DEBUG: Display Timer + Cropping Values
      if(debugFlag){

	// "cropImage()" Timer
	clock_t begin = clock();
	tempImage = cropImage(image, source_minx, source_miny, source_maxx, source_maxy, temp_minx, temp_miny, temp_maxx, temp_maxy);
	clock_t end = clock();
	
	
	// Calculate Time Elapsed
	totalTimeElapsed += double(diffclock(end, begin));
	
      } else {
	
	// Create subimage based off "Cropping Values" and place the result into tempImage
	tempImage = cropImage(image, source_minx, source_miny, source_maxx, source_maxy, temp_minx, temp_miny, temp_maxx, temp_maxy);
	
      }

      
      // Store sub-image into tiledImages array 
      tiledImages[(int)calcHeight-1-j][i] = new osg::Image;
      tiledImages[(int)calcHeight-1-j][i] = tempImage.get();

    }
  }
  
  if(debugFlag){
    std::cout << "Average Time to Scale: " << totalTimeElapsed / (calcWidth + calcHeight) << " ms"<< std::endl;
    std::cout << "Total Time to Scale: " << totalTimeElapsed << " ms"<< std::endl;
  }
  
}



void Tiles::Test(){
  
  std::cout << "Image: " << imageFileName << ", Debug: " << debugFlag << std::endl;
  std::cout << "Image Dimensions: (" << panoWidth << "x" << panoHeight << ")" << std::endl;
  std::cout << "xRes: " << xResolution << ", yRes: " << yResolution << std::endl;
  std::cout << "calcWidth: " << calcWidth << ", calcHeight: " << calcHeight << ", Time: " << totalTimeElapsed << std::endl;
  
}


osg::ref_ptr<osg::Image>** Tiles::getAllTiles(){return tiledImages;}


double Tiles::diffclock(clock_t clock1, clock_t clock2){ 
  double diffticks = clock1 - clock2; 
  double diffms = (diffticks * 1000) / CLOCKS_PER_SEC; 
  return diffms; 
  
}
  

  
osg::Image* Tiles::cropImage(const osg::Image* image, double src_minx, double src_miny, double src_maxx, double src_maxy, double &dst_minx, double &dst_miny, double &dst_maxx, double &dst_maxy){
  
    // Null Image
    if ( image == 0L ){
        return 0L;
    }

    // Image cropping values
    int windowX = src_minx;
    int windowY = src_miny;
    int windowHeight = src_maxy - src_miny;
    int windowWidth  = src_maxx - src_minx;
    
    
    //Allocate/Format the cropped image
    osg::Image* cropped = new osg::Image;
    cropped->allocateImage(windowWidth, windowHeight, image->r(), image->getPixelFormat(), image->getDataType());
    cropped->setInternalTextureFormat( image->getInternalTextureFormat() );
    
    
    // Copy sub-section of image
    for (int layer = 0; layer < image->r(); ++layer){
      for (int src_row = windowY, dst_row = 0; dst_row < windowHeight; src_row++, dst_row++){
	
	const void* src_data = image->data(windowX, src_row, layer);
	void* dst_data = cropped->data(0, dst_row, layer);
	memcpy( dst_data, src_data, cropped->getRowSizeInBytes());
	
      }
    }
    
    // Return Cropped Image
    return cropped;
}
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
 