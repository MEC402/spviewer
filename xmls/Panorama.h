
#ifndef PANORAMA_H_
#define PANORAMA_H_  
#include <osg/Image>
#include "tiles.h"


class Panorama { 
  
public:
  
  Panorama(std::string id, std::string name, std::string left_dir, std::string right_dir){
    this->id = id;
    this->name = name;
    this->left_dir = left_dir;
    this->right_dir = right_dir;
    
    
    left  = new Tiles(left_dir,  512, 512, false);
    right = new Tiles(right_dir, 512, 512, false);
    
    left->createTiles();
    right->createTiles();
    
    leftImages = left->getAllTiles();
    rightImages = right->getAllTiles();
  }
  
  
  ~Panorama(){};
  
  
  std::string getID(){ return id; }
  std::string getName(){ return name; }
  std::string getLeftDir(){ return left_dir; }
  std::string getRightDir(){ return right_dir; }
  
  osg::ref_ptr<osg::Image>** getLeftImages(){ return leftImages; }
  osg::ref_ptr<osg::Image>** getRightImages(){ return rightImages; }
  
  int getNumColumns(){ return left->getNumColumns(); };
  int getNumRows(){ return left->getNumRows(); };
  
  

private:
  
  // Panorama Values
  std::string id;
  std::string name;
  std::string left_dir;
  std::string right_dir;
  
  
  Tiles* left;
  Tiles* right;
  
  osg::ref_ptr<osg::Image>** leftImages;
  osg::ref_ptr<osg::Image>** rightImages;
};

#endif
