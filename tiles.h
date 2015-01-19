/*
 * tiles.h: this class crops an image into different images with the desired resolution and return their references 
 * 
 */

#ifndef TILES_H_
#define TILES_H_
class Tiles { 
  
public:
  
  // Constructor/Destructor
  Tiles(std::string imageFile, int cutWidth, int cutHeight, bool debug);
  Tiles(std::string imageFile, int cutWidth, int cutHeight, int xRes, int yRes, bool scale, bool debug);
  ~Tiles();
  
  // Functions
  void createTiles();
  osg::ref_ptr<osg::Image>** getAllTiles();
  int getNumColumns();
  int getNumRows();
  
  void Test();
  


private:
  
  // Functions
  double diffclock(clock_t clock1, clock_t clock2);
  osg::Image* cropImage(const osg::Image* image, double src_minx, double src_miny, double src_maxx, double src_maxy, double &dst_minx, double &dst_miny, double &dst_maxx, double &dst_maxy);
  
  
  // Panorama Vars
  int cutWidth;
  int cutHeight;
  int panoWidth;
  int panoHeight;
  
  // Image File
  std::string imageFileName;
  
  // Scale Resolution Values
  bool scaleFlag;
  int xResolution;
  int yResolution;
  
  // Calculated Values
  float calcWidth;
  float calcHeight;
  
  // Debug Values
  bool debugFlag;
  double totalTimeElapsed;
  
  // Array of Tiled Images
  osg::ref_ptr<osg::Image>** tiledImages;

};
#endif

