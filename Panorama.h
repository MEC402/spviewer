
#ifndef PANORAMA_H_
#define PANORAMA_H_  
#include <iostream>
#include <osg/Quat>
#include <osg/Image>
#include "tiles.h"


class Panorama { 
  
public:
  
  Panorama(std::string id, std::string name, std::string left_dir, std::string right_dir){
    this->id = id;
    this->name = name;
    this->left_dir = left_dir;
    this->right_dir = right_dir;

    
    std::cerr << "Allocate Tile Sets" << std::endl;    
    left  = new Tiles(left_dir,  512, 512, false);
    right = new Tiles(right_dir, 512, 512, false);
    
    std::cerr << "Create Left Tile" << std::endl;    
    left->createTiles();
    std::cerr << "Create Right Tile" << std::endl;    
    right->createTiles();
    
    leftImages = left->getAllTiles();
    rightImages = right->getAllTiles();
  }
  
  
  ~Panorama(){};
  
  
  std::string getID(){ return id; }
  std::string getName(){ return name; }
  std::string getLeftDir(){ return left_dir; }
  std::string getRightDir(){ return right_dir; }
  osg::Quat getQuat() { return myquat; }
  void setQuat(osg::Quat aq) { myquat = aq; }
  
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
  osg::Quat myquat;
  
  
  Tiles* left;
  Tiles* right;
  
  osg::ref_ptr<osg::Image>** leftImages;
  osg::ref_ptr<osg::Image>** rightImages;
};

#endif
