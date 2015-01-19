
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "XmlReader.h"



////////////////////////////////////////
// XmlReader Constructor / Destructor //
////////////////////////////////////////

XmlReader::XmlReader(){
  
  // TODO: Make Constants
  XML_TYPE_CONTROLS = "control_scheme";
  XML_TYPE_PANOS = "panoramas";
  
}



XmlReader::~XmlReader(){}




//////////////////////////////////////////
// DEBUG: Display all XML File Contents //
//////////////////////////////////////////


void XmlReader::debug(const xmlpp::Node* node){
  
  const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(node);
  const xmlpp::TextNode* nodeText	= dynamic_cast<const xmlpp::TextNode*>(node);
  const xmlpp::CommentNode* nodeComment = dynamic_cast<const xmlpp::CommentNode*>(node);
  
  
  // Ignore whitespace
  if(nodeText && nodeText->is_white_space()) 
    return;
  
  
  // Get Node Name
  const Glib::ustring nodename = node->get_name();

  
  // Let's not say "name: text".
  if(!nodeText && !nodeComment && !nodename.empty()){
    
    const Glib::ustring namespace_prefix = node->get_namespace_prefix();

    std::cout << "Node name = ";
    
    if(!namespace_prefix.empty())
      std::cout << namespace_prefix << ":";
    
    std::cout << nodename << std::endl;
    
  } 
  // Let's say when it's text. - e.g. let's say what that white space is.
  else if(nodeText){
    std::cout << "Text Node" << std::endl;
  }

  
  //Treat the various node types differently: 
  if(nodeText){
    std::cout << "text = \"" << nodeText->get_content() << "\"" << std::endl;
    
  } else if(nodeComment) {
    std::cout << "comment = " << nodeComment->get_content() << std::endl;
    
  } else if(nodeContent) {
    std::cout << "content = " << nodeContent->get_content() << std::endl;
    
  } else if(const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node)) {
    //A normal Element node:

    //line() works only for ElementNodes.
    std::cout << "     line = " << node->get_line() << std::endl;

    //Print attributes:
    const xmlpp::Element::AttributeList& attributes = nodeElement->get_attributes();
    
    
    for(xmlpp::Element::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter) {
      
      const xmlpp::Attribute* attribute = *iter;
      const Glib::ustring namespace_prefix = attribute->get_namespace_prefix();

      std::cout << "  Attribute ";
      
      if(!namespace_prefix.empty())
        std::cout << namespace_prefix  << ":";
      
      std::cout << attribute->get_name() << " = " << attribute->get_value() << std::endl;
      
    }

    const xmlpp::Attribute* attribute = nodeElement->get_attribute("title");
    
    
    if(attribute) {
      std::cout << "title = " << attribute->get_value() << std::endl;
    }
  }
  
  
  if(!nodeContent) {
    
    //Recurse through child nodes:
    xmlpp::Node::NodeList list = node->get_children();
    
    for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter){
      
      // Recursive
      debug(*iter); 
      
    }
  }
}




//////////////////////////////////////
// Parse Controls / Parse Panoramas //
//////////////////////////////////////

void XmlReader::parseControls(const xmlpp::Node* node){
  
  // Get Current Node's Content
  const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(node);
  
  // Get Node Name
  const Glib::ustring nodename = node->get_name();
  

  // If the current Node is "control".
  if(nodename.compare("control") == 0){
    
    // Get all Control Attributes
    if(const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node)) {
      
      
      // Control Variables
      std::string id = "";
      std::string keyvalue = "";
      
      
      // Get Attributes
      const xmlpp::Element::AttributeList& attributes = nodeElement->get_attributes();
      for(xmlpp::Element::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter) {
	const xmlpp::Attribute* attribute = *iter;
	
	
	// Get Control ID Value
	if(attribute->get_name().compare("id") == 0){
	 id = attribute->get_value();
	  
	// Else: Get Key Value
	} else if(attribute->get_name().compare("keyvalue") == 0){
	  keyvalue = attribute->get_value();
	}
	
      }
      
      
      // If the Values aren't empty, add to Vector
      if(id.compare("") != 0 && keyvalue.compare("") != 0){
	controlVec.addControl(id, keyvalue);
      }
    }
  }

  // If there are more Nodes, Recurse through them all
  if(!nodeContent) {
    
    //Recurse through child nodes:
    xmlpp::Node::NodeList list = node->get_children();
    
    for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter){
      parseControls(*iter); 
    }
  }
}



void XmlReader::parsePanoramas(const xmlpp::Node* node){
  
  // Get Current Node's Content
  const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(node);
  
  // Get Node Name
  const Glib::ustring nodename = node->get_name();
  

  // If the current Node is "pano"
  if(nodename.compare("pano") == 0){
    
    
    // Get all Pano Attributes
    if(const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node)) {
      
      // Panorama Variables
      std::string id = "";
      std::string name = "";
      std::string left_dir = "";
      std::string right_dir = "";
      std::string rotate="";
      
      
      // Get Attributes
      const xmlpp::Element::AttributeList& attributes = nodeElement->get_attributes();
      for(xmlpp::Element::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter) {
	const xmlpp::Attribute* attribute = *iter;
	
	
	// Get Pano ID Value
	if(attribute->get_name().compare("id") == 0){
	  id = attribute->get_value();
	  
	// Else: Get Pano Name
	} else if(attribute->get_name().compare("name") == 0){
	  name = attribute->get_value();
	
	// Else: Get Left Pano Directory
	} else if(attribute->get_name().compare("left") == 0){
	  left_dir = attribute->get_value();
	
	// Else: Get Right Pano Directory
	} else if(attribute->get_name().compare("right") == 0){
	  right_dir = attribute->get_value();
	}
	//TODO 11/2
	else if (attribute->get_name().compare("rotate")==0){
  	  rotate = attribute->get_value();

	  
	}
      }
      
      if(rotate.compare("")==0)
	std::cout<<"no rotate Attribute"<<std::endl;
      
      // If the Values aren't empty, add to Vector
      if(id.compare("") != 0 && name.compare("") != 0 && left_dir.compare("") != 0 && right_dir.compare("") != 0){
	
	//TODO
	std::cout << "Loading Panorama Data/Images: \"" << id << "\"" << std::endl;
	
	Panorama* temp = new Panorama(id, name, left_dir, right_dir);
	
	panoVec.push_back(temp);
      }
    }
  }
  

  // If there are more Nodes, Recurse through them all
  if(!nodeContent) {
    
    //Recurse through child nodes:
    xmlpp::Node::NodeList list = node->get_children();
    
    for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter){
      parsePanoramas(*iter); 
    }
  }
}



/////////////////////////////
// Determine XML File Type //
/////////////////////////////

std::string XmlReader::checkXMLFileType(const xmlpp::Node* node){

  // Get Node Name
  const std::string nodename = node->get_name();
  
  // Compare Node name to XML Types
  if(nodename.compare(XML_TYPE_CONTROLS) == 0){
      
      return XML_TYPE_CONTROLS;
      
  } else if(nodename.compare(XML_TYPE_PANOS) == 0){
      
      return XML_TYPE_PANOS;
      
  } else {
      return "OTHER";
  }
  
}




