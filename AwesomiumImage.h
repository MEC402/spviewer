/*
 * 
 * The AwesomiumImage class that is responsible for displaying the menu
 * 
 * 
 * 
 */
#ifndef AWESOMIUM_IMAGE_H_
#define AWESOMIUM_IMAGE_H_



#ifdef USE_AWESOMIUM_C_API
#include <Awesomium/awesomium_capi.h>
#include <Awesomium/KeyboardCodes.h>
#else
#include <Awesomium/STLHelpers.h>
#include <Awesomium/WebKeyboardCodes.h>
#include <Awesomium/WebKeyboardEvent.h>
#include <Awesomium/BitmapSurface.h>
#include <Awesomium/WebCore.h>
#include <Awesomium/WebView.h>
#include <Awesomium/JSObject.h>

#include <Awesomium/WebString.h>
#include <iostream>
#include <osg/Group>
#include <osg/Image>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osgGA/GUIEventHandler>
using namespace Awesomium;
#endif

//TODO
#include "js_delegate.h"
#include "MethodDispatcher.h"

#include <iostream>
#include <osg/ShapeDrawable>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include "Panorama.h"
#include "ivSphere.h"
#include "imageProcess.h"


class AwesomiumImage : public osg::Image, public imageProcess{
  
public:
  
  AwesomiumImage();
  AwesomiumImage( osg::Group* , Awesomium::WebCore*, osg::Group*, std::vector<Panorama*>, int, int*, int*, bool);
  AwesomiumImage( const AwesomiumImage& copy, const osg::CopyOp& op );
  
  META_Object( osg, AwesomiumImage );
  
  void loadURL( const std::string& url, int w, int h );
  int getWidth(){return width;};
  int getHeight(){return height;};
  void loadNextImage();
  void loadPrevImage();

  virtual bool requiresUpdateCall() const;
  virtual void update( osg::NodeVisitor* nv );
  virtual bool sendPointerEvent( int x, int y, int buttonMask );
  virtual bool sendKeyEvent( int key, bool keyDown );
  
  void BindMethods(WebView* web_view) {
    
    JSValue result = web_view->CreateGlobalJavascriptObject(WSLit("spViewer"));
    
    // Bind Custom JS Function to Function
    if (result.IsObject()) {
      JSObject& app_object = result.ToObject();
      method_dispatcher.Bind(app_object,  WSLit("openPano"), JSDelegate(this, &AwesomiumImage::OnOpenPano));
    }
    
    // Bind our method dispatcher to the WebView
    web_view->set_js_method_handler(&method_dispatcher);
  };
  
  void OnOpenPano(WebView* caller, const JSArray& args);
    
private:
  WebCore* g_webCore;
  std::string oldurl;
  std::string newurl;
  
  MethodDispatcher method_dispatcher;
  
  bool flag;
  int size;
  //the node for right sphere
  osg::Switch* rightSphere;
  
  //the geode nodes for displaying pictures on left and right sphere
  std::vector<osg::ref_ptr<osg::Group>> lsphere;
  std::vector<osg::ref_ptr<osg::Group>> rsphere;
  
  //the group node that responsible for rotating pictures on left sphere
  osg::Switch* leftSphere;
  
  // Panoramas
  std::vector<Panorama*> panos;
  
  
  //number of rows and colums for tiled images for each eyes
  int* col;
  int* row;
  
  //the width and height of the screen
  unsigned int width, height;

protected:
  virtual ~AwesomiumImage();
  
#ifdef USE_AWESOMIUM_C_API
  void injectAwesomiumKey( awe_webkey_type type, int key );
#else
  void injectAwesomiumKey( WebKeyboardEvent::Type type, int key );
#endif
  
#ifdef USE_AWESOMIUM_C_API
  awe_webview* _webView;
#else
  WebView* _webView;
#endif

  int _lastButtonMask;
  
};

#endif