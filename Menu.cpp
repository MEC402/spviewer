#include "Menu.h"

Menu::Menu(std::string url , osg::Group* pat, std::vector<Panorama*> pano, int size, int* row, int* col, bool isXinerama){
  
#ifdef USE_AWESOMIUM_C_API
  awe_webcore_initialize_default();
#else
  g_webCore = WebCore::Initialize( WebConfig() );
#endif
  
  node = new osg::Switch;
  node->setDataVariance(osg::Object::DYNAMIC);
  
 image = new AwesomiumImage(node.get(), g_webCore, pat, pano, size, row, col, isXinerama);
    image->loadURL(url, image->getWidth()/2, image->getHeight()/2);
    
  osg::ref_ptr<InteractiveHandler> handler = new InteractiveHandler( image.get(), image->getWidth(),image->getHeight() );
    
  osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
    texture->setImage(image.get());
    texture->setResizeNonPowerOfTwoHint( false );
	
  osg::ref_ptr<osg::Geometry> quad = osg::createTexturedQuadGeometry(osg::Vec3(-0.5f, -0.5f, -0.0f), osg::X_AXIS, osg::Y_AXIS);
    quad->setEventCallback( handler.get() );
    quad->setCullCallback( handler.get() );

  osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable( quad.get() );
    geode->getOrCreateStateSet()->setTextureAttributeAndModes( 0, texture.get() );
    geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
    geode->getOrCreateStateSet()->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );

  // Add HUD Camera  
  camera = new osg::Camera;
    camera->setClearMask( GL_DEPTH_BUFFER_BIT );
    camera->setRenderOrder( osg::Camera::POST_RENDER );
    camera->setReferenceFrame( osg::Camera::ABSOLUTE_RF );
    camera->setViewMatrixAsLookAt( osg::Vec3(0.0f, 0.0f, 0.0f), osg::Vec3(0.0f, 0.0f, 1.0f), osg::Vec3(0.0f, -1.0f, 0.0f));
    camera->addChild( geode.get() ); 
}


osg::Camera* Menu::getCamera(){
  return camera.get();
}


osg::Group* Menu::getGroupNode(){
  return node.get();
}


void Menu::updateMenu(){
#ifdef USE_AWESOMIUM_C_API
  awe_webcore_update();
#else
  g_webCore->Update();
#endif
}


void Menu::shutDown(){
#ifdef USE_AWESOMIUM_C_API
  awe_webcore_shutdown();
#else
  //WebCore::Shutdown();
#endif
}
