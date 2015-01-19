

#include <math.h>
#include <iostream>
#include <osg/PositionAttitudeTransform>
#include <osgDB/WriteFile>
#include "AwesomiumImage.h"
#include "Panorama.h"
#include <cstdlib>
#include <iostream>
#include <string.h>

//  #include <X11/extensions/Xinerama.h>


//create a part of sphere with texture
osg::ref_ptr<osg::Geode>  createGeode(std::string dir, int row, int column, int numRows, int numCols, osg::ref_ptr<osg::Image>** images, int nodeMask)
{
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

void AwesomiumImage::loadNextImage(){

  if (getIndex()==-1){

	// Get the number of columns and rows from the Panorama
	int cols = panos[0]->getNumColumns();
	int rows = panos[0]->getNumRows();
	*col = cols;
	*row = rows;
	
	//create the ivsphere's with texture
	lsphere.push_back( new osg::Group);
	rsphere.push_back( new osg::Group);
	
	osg::PositionAttitudeTransform* pat= new osg::PositionAttitudeTransform;

	
	osg::ref_ptr<osg::PositionAttitudeTransform>* leftrotate = new osg::ref_ptr<osg::PositionAttitudeTransform>[rows * cols];

	for (int i = 0; i < rows; i++){
	  for (int j = 0; j < cols; j++){	    
	    leftrotate[j + i * cols] = new osg::PositionAttitudeTransform;
	    leftrotate[j + i * cols]->addChild(createGeode(newurl, i, j, rows, cols, panos[0]->getLeftImages(), 0x01));
	    lsphere[0]->addChild(leftrotate[j + i * cols].get());
	    
   	    //pat=(osg::PositionAttitudeTransform*)leftrotate[j + i * cols]->getChild();	   	          	
   	    pat=(osg::PositionAttitudeTransform*)lsphere[0]->getChild(j + i * cols);	   
	     
	    pat->setAttitude(myQuat[0]);
	 
	    
	    rsphere[0]->addChild( createGeode(newurl, i, j, rows, cols, panos[0]->getRightImages(), 0x02)); 
	  }	
	}
	rightSphere->addChild(rsphere[0].get());
	leftSphere->addChild(lsphere[0].get());
	setIndex(0);
	return;
      
    }else{

	int a= getIndex();
	setIndex((a+1)%panos.size());
	a= getIndex();
	
	//if the pana was not created yet, create it
	if (a>=lsphere.size()){
	  int cols = panos[a]->getNumColumns();
	  int rows = panos[a]->getNumRows();
	  *col = cols;
	  *row = rows;
	  
	  //create the ivsphere's with texture
	  lsphere.push_back( new osg::Group);
	  rsphere.push_back( new osg::Group);
	  
	  osg::PositionAttitudeTransform* pat= new osg::PositionAttitudeTransform;

	  
	  osg::ref_ptr<osg::PositionAttitudeTransform>* leftrotate = new osg::ref_ptr<osg::PositionAttitudeTransform>[rows * cols];

	  for (int i = 0; i < rows; i++){
	    for (int j = 0; j < cols; j++){	    
	      leftrotate[j + i * cols] = new osg::PositionAttitudeTransform;
	      leftrotate[j + i * cols]->addChild(createGeode(newurl, i, j, rows, cols, panos[a]->getLeftImages(), 0x01));
	      lsphere[a]->addChild(leftrotate[j + i * cols]);
	      pat=(osg::PositionAttitudeTransform*)leftrotate[j + i * cols];	   	          	

	      pat->setAttitude(myQuat[a]);
	      
	      rsphere[a]->addChild( createGeode(newurl, i, j, rows, cols, panos[a]->getRightImages(), 0x02)); 
	    }	
	  }
	  rightSphere->addChild(rsphere[a].get());
	  leftSphere->addChild(lsphere[a].get());
	  leftSphere->setSingleChildOn(a);
	  rightSphere->setSingleChildOn(a);
	  return;
	  
	}
	else{
	  int cols = panos[a]->getNumColumns();
	  int rows = panos[a]->getNumRows();
	  *col = cols;
	  *row = rows;
  	  osg::PositionAttitudeTransform* pat= new osg::PositionAttitudeTransform;

	  for (int i = 0; i < rows; i++){
	      for (int j = 0; j < cols; j++){	    
		//leftrotate[j + i * cols] = new osg::PositionAttitudeTransform;
		//lsphere[a]->addChild(leftrotate[j + i * cols]);
		
		pat=(osg::PositionAttitudeTransform*)lsphere[a]->getChild(j + i * cols);	   	          	

		pat->setAttitude(myQuat[a]);
		
	      }	
	    }
	      leftSphere->setSingleChildOn(a);
	  rightSphere->setSingleChildOn(a);
	  
	}
	
    }
    
    
  
}

void AwesomiumImage::loadPrevImage(){
  
  
  if (getIndex()==-1){

	// Get the number of columns and rows from the Panorama
	int cols = panos[0]->getNumColumns();
	int rows = panos[0]->getNumRows();
	*col = cols;
	*row = rows;
	
	//create the ivsphere's with texture
	lsphere.push_back( new osg::Group);
	rsphere.push_back( new osg::Group);
	
	osg::PositionAttitudeTransform* pat= new osg::PositionAttitudeTransform;

	
	osg::ref_ptr<osg::PositionAttitudeTransform>* leftrotate = new osg::ref_ptr<osg::PositionAttitudeTransform>[rows * cols];

	for (int i = 0; i < rows; i++){
	  for (int j = 0; j < cols; j++){	    
	    leftrotate[j + i * cols] = new osg::PositionAttitudeTransform;
	    leftrotate[j + i * cols]->addChild(createGeode(newurl, i, j, rows, cols, panos[0]->getLeftImages(), 0x01));
	    lsphere[0]->addChild(leftrotate[j + i * cols].get());
	    
   	    //pat=(osg::PositionAttitudeTransform*)leftrotate[j + i * cols]->getChild();	   	          	
   	    pat=(osg::PositionAttitudeTransform*)lsphere[0]->getChild(j + i * cols);	   
	     
	    pat->setAttitude(myQuat[0]);
	 
	    
	    rsphere[0]->addChild( createGeode(newurl, i, j, rows, cols, panos[0]->getRightImages(), 0x02)); 
	  }	
	}
	rightSphere->addChild(rsphere[0].get());
	leftSphere->addChild(lsphere[0].get());
	setIndex(0);
	return;
      
    }else{

	int a= getIndex();
	setIndex((a-1+lsphere.size())%panos.size());
	a= getIndex();
	
	//if the pana was not created yet, create it
	if (a>=lsphere.size()){
	  int cols = panos[a]->getNumColumns();
	  int rows = panos[a]->getNumRows();
	  *col = cols;
	  *row = rows;
	  
	  //create the ivsphere's with texture
	  lsphere.push_back( new osg::Group);
	  rsphere.push_back( new osg::Group);
	  
	  osg::PositionAttitudeTransform* pat= new osg::PositionAttitudeTransform;

	  
	  osg::ref_ptr<osg::PositionAttitudeTransform>* leftrotate = new osg::ref_ptr<osg::PositionAttitudeTransform>[rows * cols];

	  for (int i = 0; i < rows; i++){
	    for (int j = 0; j < cols; j++){	    
	      leftrotate[j + i * cols] = new osg::PositionAttitudeTransform;
	      leftrotate[j + i * cols]->addChild(createGeode(newurl, i, j, rows, cols, panos[a]->getLeftImages(), 0x01));
	      lsphere[a]->addChild(leftrotate[j + i * cols]);
	      pat=(osg::PositionAttitudeTransform*)leftrotate[j + i * cols];	   	          	

	      pat->setAttitude(myQuat[a]);
	      
	      rsphere[a]->addChild( createGeode(newurl, i, j, rows, cols, panos[a]->getRightImages(), 0x02)); 
	    }	
	  }
	  rightSphere->addChild(rsphere[a].get());
	  leftSphere->addChild(lsphere[a].get());
	  leftSphere->setSingleChildOn(a);
	  rightSphere->setSingleChildOn(a);
	  return;
	  
	}
	else{
	  int cols = panos[a]->getNumColumns();
	  int rows = panos[a]->getNumRows();
	  *col = cols;
	  *row = rows;
  	  osg::PositionAttitudeTransform* pat= new osg::PositionAttitudeTransform;

	  for (int i = 0; i < rows; i++){
	      for (int j = 0; j < cols; j++){	    
		//leftrotate[j + i * cols] = new osg::PositionAttitudeTransform;
		//lsphere[a]->addChild(leftrotate[j + i * cols]);
		
		pat=(osg::PositionAttitudeTransform*)lsphere[a]->getChild(j + i * cols);	   	          	

		pat->setAttitude(myQuat[a]);
		
	      }	
	    }
	      leftSphere->setSingleChildOn(a);
	  rightSphere->setSingleChildOn(a);
	  
	}
	
    }
    
    
  
}
void AwesomiumImage::OnOpenPano(WebView* caller, const JSArray& args) {

  //TODO ERROR (Too Many/Little Arguments)
  if(args.size() != 1){
    
    if(args.size() < 1){
      std::cout << "Too Few Arguments for Panorama." << std::endl;
      
    } else {
      std::cout << "Too Many Arguments for Panorama: (" << "";
      
      for(int i = 0; i < args.size(); i++){
	std::cout <<  args[i].ToString() << " ";
      }
      
      std::cout << ")" << std::endl;
    }
    
    return;
  }
  
  // Selected Pano ID
  std::string selectedPanoID = ToString(args[0].ToString());

  //TODO Check which pano to load
  for(int a = 0; a < panos.size(); a++){
    if(panos[a]->getID().compare(selectedPanoID) == 0){
	 if (a>=lsphere.size()){
	  int cols = panos[a]->getNumColumns();
	  int rows = panos[a]->getNumRows();
	  *col = cols;
	  *row = rows;
	  
	  //create the ivsphere's with texture
	  lsphere.push_back( new osg::Group);
	  rsphere.push_back( new osg::Group);
	  
	  osg::PositionAttitudeTransform* pat= new osg::PositionAttitudeTransform;

	  
	  osg::ref_ptr<osg::PositionAttitudeTransform>* leftrotate = new osg::ref_ptr<osg::PositionAttitudeTransform>[rows * cols];

	  for (int i = 0; i < rows; i++){
	    for (int j = 0; j < cols; j++){	    
	      leftrotate[j + i * cols] = new osg::PositionAttitudeTransform;
	      leftrotate[j + i * cols]->addChild(createGeode(newurl, i, j, rows, cols, panos[a]->getLeftImages(), 0x01));
	      lsphere[a]->addChild(leftrotate[j + i * cols]);
	      pat=(osg::PositionAttitudeTransform*)leftrotate[j + i * cols];	   	          	

	      pat->setAttitude(myQuat[a]);
	      
	      rsphere[a]->addChild( createGeode(newurl, i, j, rows, cols, panos[a]->getRightImages(), 0x02)); 
	    }	
	  }
	  rightSphere->addChild(rsphere[a].get());
	  leftSphere->addChild(lsphere[a].get());
	  leftSphere->setSingleChildOn(a);
	  rightSphere->setSingleChildOn(a);
	  return;
	  
	}
	else{
	  int cols = panos[a]->getNumColumns();
	  int rows = panos[a]->getNumRows();
	  *col = cols;
	  *row = rows;
  	  osg::PositionAttitudeTransform* pat= new osg::PositionAttitudeTransform;

	  for (int i = 0; i < rows; i++){
	      for (int j = 0; j < cols; j++){	    
		//leftrotate[j + i * cols] = new osg::PositionAttitudeTransform;
		//lsphere[a]->addChild(leftrotate[j + i * cols]);
		
		pat=(osg::PositionAttitudeTransform*)lsphere[a]->getChild(j + i * cols);	   	          	

		pat->setAttitude(myQuat[a]);
		
	      }	
	    }
	      leftSphere->setSingleChildOn(a);
	  rightSphere->setSingleChildOn(a);
	  
	}
	
//       if (node != NULL){
// 	
// 	if(node->getNumChildren() != 0){
// 	  
// 	  // Remove all Children
// 	  int num = node->getNumChildren();
// 	  node->removeChildren(0, num);
// 	  
// 	}
// 	//get the index
// 	setIndex(a);
// 	
// 	// Remove all Children of "rotate"
// 	if(rotate->getNumChildren() != 0){
// 	  rotate->removeChildren(0, rotate->getNumChildren());
// 	}
// 	
// 	// Get the number of columns and rows from the Panorama
// 	int cols = panos[a]->getNumColumns();
// 	int rows = panos[a]->getNumRows();
// 	*col = cols;
// 	*row = rows;
// 	
// 	//create the ivsphere's with texture
// 	lsphere = new osg::ref_ptr<osg::Geode>[cols * rows];
// 	rsphere = new osg::ref_ptr<osg::Geode>[cols * rows];
// 	
// 	osg::PositionAttitudeTransform* pat= new osg::PositionAttitudeTransform;
// 
// 	
// 	osg::ref_ptr<osg::PositionAttitudeTransform>* leftrotate = new osg::ref_ptr<osg::PositionAttitudeTransform>[rows * cols];
// 	for (int i = 0; i < rows; i++){
// 	  for (int j = 0; j < cols; j++){
// 	    
// 	    lsphere[j + i * cols] = createGeode(newurl, i, j, rows, cols, panos[a]->getLeftImages(), 0x01);
// 	    leftrotate[j + i * cols] = new osg::PositionAttitudeTransform;
// 	    leftrotate[j + i * cols]->addChild(lsphere[j + cols * i].get());
// 	    rotate->addChild(leftrotate[j + i * cols].get());
// 	    
// 	    //TODO: 11/6 SET THE rotation of left sphere
//    	    pat=(osg::PositionAttitudeTransform*)rotate->getChild(j + i * cols);
// 	    //get the rotate degree
// 	    double d=rotatedLeftSphere[a];
// 	    double d1=verticalRotation[a];
//     	    //std::cout<<d<<std::endl;
// 
// 	    //while(d<0.0001){
// 	    osg::Quat x= osg::Quat(d, osg::Vec3d(0.0f,0.0f,1.0f));
// 	    x = x*osg::Quat(-d1, osg::Vec3d(10.0,0.0,0.0));
// 	    pat->setAttitude(x);
// 	    //}
// 	    
// 	    
// 	    rsphere[j + i * cols] = createGeode(newurl, i, j, rows, cols, panos[a]->getRightImages(), 0x02);
// 	    node->addChild(rsphere[j + i * cols].get());
// 	  }
// 	}
//       }
//       return;
      
    } else { 
      if(a == (panos.size() - 1)){
	std::cout << "Pano \"" << selectedPanoID << "\" not found within Panorama XML File" << std::endl;
      } 
    }
  }
}





// //----------------------------//
// AwesomiumImage Constructor //
//----------------------------//

AwesomiumImage::AwesomiumImage()
  : _webView(0)
  , _lastButtonMask(0)
  , oldurl("")
  , newurl("") 
{}




AwesomiumImage::AwesomiumImage(osg::Group* b, WebCore* c, osg::Group* pat, std::vector<Panorama*> panos, int _size, int* _row, int* _col, bool isXinerama)
  : _webView(0)
  , _lastButtonMask(0)
  , oldurl("")
  , newurl("") 
{
  g_webCore = c;
  rightSphere = (osg::Switch*)b;
  //node->setDataVariance(osg::Object::DYNAMIC);

  leftSphere = (osg::Switch*) pat;
  this->panos = panos;
  size = _size;
  col = _col;
  row = _row;
  
//TEST
//   bool success=false;
//   Display *d=XOpenDisplay(NULL);
//   if (d) {
// 	int dummy1, dummy2;
// 	if (XineramaQueryExtension(d, &dummy1, &dummy2)) {
// 		if (XineramaIsActive(d)) {
// 			int heads=0;
// 			XineramaScreenInfo *p=XineramaQueryScreens(d, &heads);
// 			if (heads>0) {
// 				for (int x=0; x<heads; ++x)
// 					std::cout << "Head " << x+1 << " of " << heads << ": " <<
// 						p[x].width << "x" << p[x].height << " at " <<
// 						p[x].x_org << "," << p[x].y_org << std::endl;
// 				success=true;
// 			} else std::cout << "XineramaQueryScreens says there aren't any" << std::endl;
// 			XFree(p);
// 		} else std::cout << "Xinerama not active" << std::endl;
// 	} else std::cout << "No Xinerama extension" << std::endl;
// 	XCloseDisplay(d);
//   } else std::cout << "Can't open display" << std::endl;
//   
//   
//   
//   
//   
  
  
  osg::GraphicsContext::WindowingSystemInterface* wsi = osg::GraphicsContext::getWindowingSystemInterface();
  
  if (!wsi)
  {
    osg::notify(osg::NOTICE)<<"Error, no WindowSystemInterface available, cannot create windows."<<std::endl;
    return ;
    
  }
  
  wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);
 
  
  // Divide Screen width when working with Xinerama
  if(isXinerama){
    width = width / 2;
  }
  
  std::cout << "Computed Screen Resolution: (" << width << "x" << height << ")\t <-- If this is incorrect, try using the \"-xin\" flag." << std::endl;

  
  if(width == 0 && height == 0) {
    width =  960 * 2;
    height = 540 * 2;
  }
}



AwesomiumImage::AwesomiumImage(const AwesomiumImage& copy, const osg::CopyOp& op)
  : osg::Image(copy, op)
  , _webView(copy._webView)
  , _lastButtonMask(copy._lastButtonMask) 
{}
    

void AwesomiumImage::loadURL(const std::string& url, int w, int h)
{
  
#ifdef USE_AWESOMIUM_C_API
  
  if ( !_webView ){
    _webView = awe_webcore_create_webview(w > 0 ? w : 512, h > 0 ? h : 512, false);
    awe_webview_set_transparent(_webView, true);
      
  } else if ( w > 0 && h > 0 ) {
    awe_webview_resize(_webView, w, h, true, 300);
  }
  
  awe_string* aweURL = awe_string_create_from_ascii(url.c_str(), url.size());
  awe_webview_load_url(_webView, aweURL, awe_string_empty(), awe_string_empty(), awe_string_empty());
  

  
  awe_string_destroy( aweURL );
  awe_webview_focus( _webView );

#else

  if ( !_webView ){
    _webView = g_webCore->CreateWebView(w > 0 ? w : 512, h > 0 ? h : 512);
    _webView->SetTransparent(true);
    
  } else if (w > 0 && h > 0) {
    _webView->Resize(w, h);
  }
  
  
  //TODO Binding Javascript Functions to C++ Functions
  BindMethods(_webView);
  
  WebURL urlData(WSLit(url.c_str()));

  _webView->LoadURL(urlData);
  _webView->Focus();

#endif
  

}

    
    
bool AwesomiumImage::requiresUpdateCall() const { return true; }
    
    
    
void AwesomiumImage::update( osg::NodeVisitor* nv )
{
  
  if ( !_webView ) return;

#ifdef USE_AWESOMIUM_C_API

  const awe_renderbuffer* buffer = awe_webview_render(_webView);
  setImage(awe_renderbuffer_get_width(buffer), awe_renderbuffer_get_height(buffer), 1, 4, GL_BGRA, GL_UNSIGNED_BYTE, const_cast<unsigned char*>(awe_renderbuffer_get_buffer(buffer)), osg::Image::NO_DELETE );

#else

  BitmapSurface* surface = static_cast<BitmapSurface*>( _webView->surface() );
  
  if ( surface && surface->is_dirty() )
  {
      setImage( surface->width(), surface->height(), 1, 4, GL_BGRA, GL_UNSIGNED_BYTE, const_cast<unsigned char*>(surface->buffer()), osg::Image::NO_DELETE );
  }
        
#endif
}



bool AwesomiumImage::sendPointerEvent( int x, int y, int buttonMask )
{
  
  if ( !_webView ) return false;
  
  
#ifdef USE_AWESOMIUM_C_API
  
  switch ( buttonMask ){
    case osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON:
	if (!_lastButtonMask) {
	  awe_webview_inject_mouse_down( _webView, AWE_MB_LEFT );
	} else {
	  awe_webview_inject_mouse_move( _webView, x, y ); 
	}
	
	break;
	
    case osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON:
	if (!_lastButtonMask) {
	  awe_webview_inject_mouse_down( _webView, AWE_MB_MIDDLE );
	} else {
	  awe_webview_inject_mouse_move( _webView, x, y );
	}
	
	break;
	
    case osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON:
	if ( !_lastButtonMask ) {
	  awe_webview_inject_mouse_down( _webView, AWE_MB_RIGHT );
	} else {
	  awe_webview_inject_mouse_move( _webView, x, y );
	}
	
	break;
	
    default:
	awe_webview_inject_mouse_move( _webView, x, y );
	
	break;
  }
  
  if (_lastButtonMask != 0 && buttonMask == 0)
  {
      switch (_lastButtonMask){
	
	case osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON:
	    awe_webview_inject_mouse_up( _webView, AWE_MB_LEFT );
	    break;
	    
	case osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON:
	    awe_webview_inject_mouse_up( _webView, AWE_MB_MIDDLE );
	    break;
	    
	case osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON:
	    awe_webview_inject_mouse_up( _webView, AWE_MB_RIGHT );
	    break;
      }
  }
        
#else

  switch ( buttonMask ){
    
    case osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON:
	if ( !_lastButtonMask ) {
	  _webView->InjectMouseDown( kMouseButton_Left );
	} else {
	  _webView->InjectMouseMove( x, y );
	}
	  
	break;
	
    case osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON:
	if ( !_lastButtonMask ) {
	  _webView->InjectMouseDown( kMouseButton_Middle );
	} else {
	  _webView->InjectMouseMove( x, y );
	}
	
	break;
	
    case osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON:
	if ( !_lastButtonMask ) {
	  _webView->InjectMouseDown( kMouseButton_Right );
	} else {
	  _webView->InjectMouseMove( x, y );
	}
	
	break;
	
    default:
	_webView->InjectMouseMove( x, y );
	
	break;
  }
  
  if ( _lastButtonMask != 0 && buttonMask == 0 ){
    switch ( _lastButtonMask ){
      
      case osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON:
	  _webView->InjectMouseUp( kMouseButton_Left );
	  break;
	  
      case osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON:
	  _webView->InjectMouseUp( kMouseButton_Middle );
	  break;
      case osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON:
	  _webView->InjectMouseUp( kMouseButton_Right );
	  break;
      }
      
      
  }

#endif

  _lastButtonMask = buttonMask;
  return true;
}
    
    
    
bool AwesomiumImage::sendKeyEvent( int key, bool keyDown )
{
  if ( !_webView ) return false;
  
  
#ifdef USE_AWESOMIUM_C_API
  awe_webkey_type type = keyDown ? AWE_WKT_KEYDOWN : AWE_WKT_KEYUP;
#else
  WebKeyboardEvent::Type type = keyDown ? WebKeyboardEvent::kTypeKeyDown : WebKeyboardEvent::kTypeKeyUp;
#endif
  
  switch ( key ){
    case osgGA::GUIEventAdapter::KEY_Tab:
	injectAwesomiumKey( type, Awesomium::KeyCodes::AK_TAB );
	break;
    case osgGA::GUIEventAdapter::KEY_BackSpace:
	injectAwesomiumKey( type, Awesomium::KeyCodes::AK_BACK );
	break;
    case osgGA::GUIEventAdapter::KEY_Return:
	injectAwesomiumKey( type, Awesomium::KeyCodes::AK_RETURN );
	break;
    case osgGA::GUIEventAdapter::KEY_Left:
	injectAwesomiumKey( type, Awesomium::KeyCodes::AK_LEFT );
	break;
    case osgGA::GUIEventAdapter::KEY_Right:
	injectAwesomiumKey( type, Awesomium::KeyCodes::AK_RIGHT );
	break;
    case osgGA::GUIEventAdapter::KEY_Up:
	injectAwesomiumKey( type, Awesomium::KeyCodes::AK_UP );
	break;
    case osgGA::GUIEventAdapter::KEY_Down:
	injectAwesomiumKey( type, Awesomium::KeyCodes::AK_DOWN );
	break;
    case osgGA::GUIEventAdapter::KEY_Page_Down:
	injectAwesomiumKey( type, Awesomium::KeyCodes::AK_NEXT );
	break;
    case osgGA::GUIEventAdapter::KEY_Page_Up:
	injectAwesomiumKey( type, Awesomium::KeyCodes::AK_PRIOR );
	break;
    case osgGA::GUIEventAdapter::KEY_Home:
	injectAwesomiumKey( type, Awesomium::KeyCodes::AK_HOME );
	break;
    case osgGA::GUIEventAdapter::KEY_End:
	injectAwesomiumKey( type, Awesomium::KeyCodes::AK_END );
	break;
    default:
	  
#ifdef USE_AWESOMIUM_C_API 
    case osgGA::GUIEventAdapter::KEY_Tab:
	injectAwesomiumKey( type, Awesomium::KeyCodes::AK_TAB );
	break;
    case osgGA::GUIEventAdapter::KEY_BackSpace:
	injectAwesomiumKey( type, Awesomium::KeyCodes::AK_BACK );
	break;
    case osgGA::GUIEventAdapter::KEY_Return:
	injectAwesomiumKey( type, Awesomium::KeyCodes::AK_RETURN );
	break;
    case osgGA::GUIEventAdapter::KEY_Left:
	injectAwesomiumKey( type, Awesomium::KeyCodes::AK_LEFT );
	break;
    case osgGA::GUIEventAdapter::KEY_Right:
	injectAwesomiumKey( type, Awesomium::KeyCodes::AK_RIGHT );
	break;
    case osgGA::GUIEventAdapter::KEY_Up:
	injectAwesomiumKey( type, Awesomium::KeyCodes::AK_UP );
	break;
    case osgGA::GUIEventAdapter::KEY_Down:
	injectAwesomiumKey( type, Awesomium::KeyCodes::AK_DOWN );
	break;
    case osgGA::GUIEventAdapter::KEY_Page_Down:
	injectAwesomiumKey( type, Awesomium::KeyCodes::AK_NEXT );
	break;
    case osgGA::GUIEventAdapter::KEY_Page_Up:
	injectAwesomiumKey( type, Awesomium::KeyCodes::AK_PRIOR );
	break;
    case osgGA::GUIEventAdapter::KEY_Home:
	injectAwesomiumKey( type, Awesomium::KeyCodes::AK_HOME );
	break;
    case osgGA::GUIEventAdapter::KEY_End:
	injectAwesomiumKey( type, Awesomium::KeyCodes::AK_END );
	break;
    default:
	if ( keyDown ) injectAwesomiumKey( AWE_WKT_CHAR, key );
#else
	if ( keyDown ) injectAwesomiumKey( WebKeyboardEvent::kTypeChar, key );
#endif
	break;
   }
   
   return true;
}
    

    
AwesomiumImage::~AwesomiumImage()
{
  if ( _webView )
#ifdef USE_AWESOMIUM_C_API
    awe_webview_destroy( _webView );
#else
    _webView->Destroy();
#endif
    
}
    
    
    
#ifdef USE_AWESOMIUM_C_API
void AwesomiumImage::injectAwesomiumKey( awe_webkey_type type, int key )
{
    awe_webkeyboardevent keyEvent;
    keyEvent.type = type;
    keyEvent.virtual_key_code = key;
    keyEvent.native_key_code = key;
    keyEvent.text[0] = (char)key; keyEvent.text[1] = '\0';
    keyEvent.unmodified_text[0] = (char)key; keyEvent.unmodified_text[1] = '\0';
    keyEvent.modifiers = 0;
    keyEvent.is_system_key = false;
    awe_webview_inject_keyboard_event( _webView, keyEvent );
}


#else
void AwesomiumImage::injectAwesomiumKey( WebKeyboardEvent::Type type, int key )
{
    WebKeyboardEvent keyEvent;
    keyEvent.type = type;
    keyEvent.virtual_key_code = key;
    keyEvent.native_key_code = key;
    keyEvent.text[0] = (char)key; keyEvent.text[1] = '\0';
    keyEvent.unmodified_text[0] = (char)key; keyEvent.unmodified_text[1] = '\0';
    keyEvent.modifiers = 0;
    keyEvent.is_system_key = false;

    _webView->InjectKeyboardEvent( keyEvent );
  
}
#endif
    
