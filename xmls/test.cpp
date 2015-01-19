 

/*
 * spViewer.cpp
 *
 *  Build file for simple stereo viewer of stereo panospheres
 */



#include <iostream>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>

#include "Panorama.h"
#include "XmlReader.h"
#include <libxml++-2.6/libxml++/libxml++.h>
#include <stdlib.h>




int main( int argc, char** argv )
{
    osg::ArgumentParser arguments(&argc, argv);
    std::string xmlFileName;

  arguments.read("--xml", xmlFileName);
  std::vector<Panorama*> panoramas;


  // Read Panorama File
  //readXMLPanoramas(xmlFileName, panoramas);
  
  try {

    // Declare Parser
    xmlpp::DomParser parser;
    
    
    // Parser Settings
    //We can have the text resolved/unescaped automatically.
    parser.set_substitute_entities(true);
    
    
    // Parse through input XML file
    parser.parse_file(xmlFileName);
   
    
    if(parser){
      
      // Parse XML Tree
      const xmlpp::Node* pNode = parser.get_document()->get_root_node();
      

      // Declare XMLReader
      XmlReader reader;
      
      // Check XML File Type
      std::string result = reader.checkXMLFileType(pNode);

      
      // Panorama XML
      if(result.compare(reader.getXMLTYPEPANOS()) == 0){
	
	//TODO
	std::cout << "Loading Panoramas..." << std::endl;

	// Parse and Load Panoramas
	reader.parsePanoramas(pNode);

	// Get Panoramas
	panoramas = reader.getPanoramas();
	
	//TODO
	std::cout << "Panoramas Loaded." << std::endl;

      // Other XML File
      } else {
	std::cout << "[ERROR] Unknown XML File: " << result << std::endl;
      }

    }
    
  } catch(const std::exception& ex) {
    
    std::cerr << "Exception caught: " << ex.what() << std::endl;
    return EXIT_FAILURE;
    
  }

  
  
  

}





