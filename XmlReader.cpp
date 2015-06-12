
#include <iostream>
#include <string>
#include <stdlib.h>
#include "XmlReader.h"

#include <stdio.h>
std::vector<double> XmlReader::rotatedLeftSphere;
std::vector<double> XmlReader::verticalRotation;
std::vector<osg::Quat> XmlReader::myQuat;

std::vector<std::string> XmlReader::Ids;
std::vector<Panorama*> XmlReader::panoVec;
const xmlpp::Node* XmlReader::node;


std::string toString(float f)
{
  char ar[10];
  sprintf(ar,"%f",f);
  return std::string(ar);
}

////////////////////////////////////////
// XmlReader Constructor / Destructor //
////////////////////////////////////////

XmlReader::XmlReader(){
  
  // TODO: Make Constants
  XML_TYPE_CONTROLS = "control_scheme";
  XML_TYPE_PANOS = "panoramas";
}




XmlReader::~XmlReader(){}


void XmlReader::setPnode(const xmlpp::Node* pnode){
  node= pnode;
  
}

//////////////////////////////////////////
// DEBUG: Display all XML File Contents //
//////////////////////////////////////////


void XmlReader::debug(){
  
  const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(node);
  const xmlpp::TextNode* nodeText	= dynamic_cast<const xmlpp::TextNode*>(node);
  const xmlpp::CommentNode* nodeComment = dynamic_cast<const xmlpp::CommentNode*>(node);
  
  
  // Ignore whitespace
  if(nodeText && nodeText->is_white_space()) 
    return;
  
  
  // Get Node Name
  const std::string nodename = node->get_name();
//  const std::string nodename = node->get_name();

  
  // Let's not say "name: text".
  if(!nodeText && !nodeComment && !nodename.empty()){
    
    const std::string namespace_prefix = node->get_namespace_prefix();

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
      const std::string namespace_prefix = attribute->get_namespace_prefix();

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

void XmlReader::debug(const xmlpp::Node* pnode){
  
  const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(pnode);
  const xmlpp::TextNode* nodeText	= dynamic_cast<const xmlpp::TextNode*>(pnode);
  const xmlpp::CommentNode* nodeComment = dynamic_cast<const xmlpp::CommentNode*>(pnode);
  
  
  // Ignore whitespace
  if(nodeText && nodeText->is_white_space()) 
    return;
  
  
  // Get Node Name
  const std::string nodename = pnode->get_name();

  
  // Let's not say "name: text".
  if(!nodeText && !nodeComment && !nodename.empty()){
    
    const std::string namespace_prefix = pnode->get_namespace_prefix();

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
    
  } else if(const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(pnode)) {
    //A normal Element node:

    //line() works only for ElementNodes.
    std::cout << "     line = " << pnode->get_line() << std::endl;

    //Print attributes:
    const xmlpp::Element::AttributeList& attributes = nodeElement->get_attributes();
    
    
    for(xmlpp::Element::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter) {
      
      const xmlpp::Attribute* attribute = *iter;
      const std::string namespace_prefix = attribute->get_namespace_prefix();

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
  
  
}




//////////////////////////////////////
// Parse Controls / Parse Panoramas //
//////////////////////////////////////

void XmlReader::parseControls(){
  
  // Get Current Node's Content
  const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(node);
  
  // Get Node Name
  const std::string nodename = node->get_name();
  

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


void XmlReader::parseControls(const xmlpp::Node* pnode){
  
  // Get Current Node's Content
  const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(pnode);
  
  // Get Node Name
  const std::string nodename = pnode->get_name();
  

  // If the current Node is "control".
  if(nodename.compare("control") == 0){
    
    // Get all Control Attributes
    if(const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(pnode)) {
      
      
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

  
}

void XmlReader::parsePanoramas(const xmlpp::Node* pnode){
  
  // Get Current Node's Content
  const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(pnode);
  
  // Get Node Name
  const std::string nodename = pnode->get_name();
  

  // If the current Node is "pano"
  if(nodename.compare("pano") == 0){
    
    
    // Get all Pano Attributes
    if(const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(pnode)) {
      
      // Panorama Variables
      std::string id = "";
      std::string name = "";
      std::string left_dir = "";
      std::string right_dir = "";
      std::string rotate="";
      std::string vertical="";
      std::string quat="";
      
      // Get Attributes
      const xmlpp::Element::AttributeList& attributes = nodeElement->get_attributes();
      for(xmlpp::Element::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter) {
	const xmlpp::Attribute* attribute = *iter;
	
	
	// Get Pano ID Value
	if(attribute->get_name().compare("id") == 0){
	  id = attribute->get_value();
	  Ids.push_back(id);
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
	else if (attribute->get_name().compare("horizontal")==0)
  	  rotate = attribute->get_value();
	else if (attribute->get_name().compare("quat")==0)
  	  quat = attribute->get_value();
	else if (attribute->get_name().compare("vertical")==0){
  	  vertical = attribute->get_value();
	 
	  
	}
      }
      
      
      
      // If the Values aren't empty, add to Vector
      if(id.compare("") != 0 && name.compare("") != 0 && left_dir.compare("") != 0 && right_dir.compare("") != 0){
	
	//TODO
	std::cout << "Loading Panorama Data/Images: \"" << id << "\"" << std::endl;
	
	Panorama* temp = new Panorama(id, name, left_dir, right_dir);
	
	panoVec.push_back(temp);
      }
      if(rotate.compare("")==0)
	rotatedLeftSphere.push_back(0.0);
      else{
	double d;
	sscanf(rotate.c_str(), "%lf", &d);
	rotatedLeftSphere.push_back(d);
	
      }
      if(quat.compare("")==0){
	myQuat.push_back(osg::Quat(0,0,0,0.1));
	//std::cout<<"adfaf\n";
	
      }else{
	//std::stringstream s(quat.c_str());
	//std::cout<<quat<<"\n";

 	char myquat [quat.size()+1];
 	quat.copy(myquat,quat.size());
 	double a[4];
 	int k=0;

 	char* c=strtok(myquat, ";");
	
 	while(c!=NULL)
 	{	
 	  a[k]=atof(c); 
	//  std::cerr<<a[k]<<"\n";

 	  c=strtok(NULL, ";");
	  k++;
 	} 
 	

//	std::string s1,s2,s3;
// 	std::cout<<"IMHERE\n";
// 	std::cout<<quat.c_str();
//	s>>a>>s1>>b>>s2>>c>>s3>>d;
//	std::cout<<a<<" "<<b<<" "<<c<<" "<<d<<"\n";
	myQuat.push_back( osg::Quat(a[0],a[1],a[2],a[3]));

      }
      
       if(vertical.compare("")==0)
	verticalRotation.push_back(0.0);
      else{
	double d;
	sscanf(vertical.c_str(), "%lf", &d);
	verticalRotation.push_back(d);
	
      }
    }
     
  }
  


} 

void XmlReader::parsePanoramas(){
  
  // Get Current Node's Content
  const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(node);
  
  // Get Node Name
  const std::string nodename = node->get_name();
  

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
      std::string vertical="";

      
      
      // Get Attributes
      const xmlpp::Element::AttributeList& attributes = nodeElement->get_attributes();
      for(xmlpp::Element::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter) {
	const xmlpp::Attribute* attribute = *iter;
	
	
	// Get Pano ID Value
	if(attribute->get_name().compare("id") == 0){
	  id = attribute->get_value();
	  Ids.push_back(id);
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
	else if (attribute->get_name().compare("horizontal")==0)
  	  rotate = attribute->get_value();
	else if (attribute->get_name().compare("vertical")==0){
  	  vertical = attribute->get_value();
	 
	  
	 
	  
	}
      }
      
      
      
      // If the Values aren't empty, add to Vector
      if(id.compare("") != 0 && name.compare("") != 0 && left_dir.compare("") != 0 && right_dir.compare("") != 0){
	
	//TODO
	std::cout << "Loading Panorama Data/Images: \"" << id << "\"" << std::endl;
	
	Panorama* temp = new Panorama(id, name, left_dir, right_dir);
	
	panoVec.push_back(temp);
      }
      if(rotate.compare("")==0)
	rotatedLeftSphere.push_back(0.0);
      else{
	double d;
	sscanf(rotate.c_str(), "%lf", &d);
	rotatedLeftSphere.push_back(d);
	
      }
      
       if(vertical.compare("")==0)
	verticalRotation.push_back(0.0);
      else{
	double d;
	sscanf(vertical.c_str(), "%lf", &d);
	verticalRotation.push_back(d);
	
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



void XmlReader::writeToFile(xmlpp::Document * d,const xmlpp::Node* pnode,std::string myid,std::string degree, std::string degreeVer){
  
//   // Get Current Node's Content
//   const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(pnode);
//   
//   // Get Node Name
//   const std::string nodename = pnode->get_name();
//   
// 
//   // If the current Node is "pano"
//   if(nodename.compare("pano") == 0){
//     
//     // Get all Pano Attributes
//     if(const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(pnode)) {
//       
//       // Panorama Variables
//       std::string id = "";
// 
//       
//       
//       // Get Attributes
//       const xmlpp::Element::AttributeList& attributes = nodeElement->get_attributes();
//       for(xmlpp::Element::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter) {
// 	 xmlpp::Attribute* attribute = *iter;
// 
// 	
// 	// Get Pano ID Value
// 	if(attribute->get_name().compare("id") == 0)
// 	  id = attribute->get_value();
// 	  
// 	  if(id.compare(myid)!=0)
// 	    continue;
// 	  else{
// 	    iter ++;
//     	    iter ++;
// 	    iter ++;
// 	    iter ++;
// 	    attribute = *iter;
// 	      if(attribute->get_name().compare("horizontal") == 0){
// 		  std::string cd=  std::string(degree.c_str());
// 		  attribute->set_value(cd);
// 	  	  attribute = *(++iter);
// 
// 		  if(attribute->get_name().compare("vertical") == 0){
// 		    cd=  std::string(degreeVer.c_str());
// 		    attribute->set_value(cd);
// 
// 		    return;
// 		    
// 		  }
// 	      }
// 	  }
//       }
//       
//       
//     
//     }
//      
//   }
  }
  




void XmlReader::writeToFile(std::string d1,std::string myid,std::string degree, std::string degreeVer){
     xmlpp::DomParser parser;
    parser.parse_file(d1);    
    xmlpp::Node* node  = parser.get_document()->get_root_node();

  // Get Current Node's Content
  const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(node);
  

  // Get Node Name 
  const std::string nodename = node->get_name();
  int i=0;

  
  
  // If the current Node is "pano"
 if(nodename.compare("pano") == 0){

    // Get all Pano Attributes
    if(const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node)) {

 

      // Get Attributes
      const xmlpp::Element::AttributeList& attributes = nodeElement->get_attributes();

      for(xmlpp::Element::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter) {
	 xmlpp::Attribute* attribute = *iter;	
	if(attribute->get_name().compare("horizontal") == 0){
//	   std::string cd=  std::string(toString(rotatedLeftSphere[i]).c_str());
	   std::string cd=  std::string(toString(rotatedLeftSphere[i])).c_str();
	  attribute->set_value(cd);
	   attribute = *(++iter);
	    
		  if(attribute->get_name().compare("vertical") == 0){
		    cd=  std::string(toString(verticalRotation[i]).c_str());
		    attribute->set_value(cd);  
		    i++;

		  }
	}
      }
      
    
    }
     
  }

  // If there are more Nodes, through them all
  if(!nodeContent) {

    // child nodes:
    xmlpp::Node::NodeList list = node->get_children();
    for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter){
      
       const xmlpp::ContentNode* nodeContent1 = dynamic_cast<const xmlpp::ContentNode*>((*iter));
       const std::string nodename1 = (*iter)->get_name();
      
      // If the current Node is "pano"
      if(nodename1.compare("pano") == 0){
	
	// Get all Pano Attributes
	if(const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(*iter)) {
	  
	  
	  // Get Attributes
	  const xmlpp::Element::AttributeList& attributes = nodeElement->get_attributes();
	  for(xmlpp::Element::AttributeList::const_iterator  iter1= attributes.begin(); iter1 != attributes.end(); ++iter1) {
		  xmlpp::Attribute* attribute1 = *iter1;	     
		  if(attribute1->get_name().compare("horizontal") == 0){
		      std::string cd=  std::string(toString(rotatedLeftSphere[i]).c_str());
		      attribute1->set_value(cd);}

		    if(attribute1->get_name().compare("vertical") == 0){
		      std::string cd=  std::string(toString(verticalRotation[i]).c_str());
		      attribute1->set_value(cd);     


		    }
		     if(attribute1->get_name().compare("quat") == 0){
		      std::string cd=  std::string((toString(myQuat[i].x())+";"+toString(myQuat[i].y())+";"+toString(myQuat[i].z())+";"+toString(myQuat[i].w())).c_str());
		      attribute1->set_value(cd);     


		    }
		  
	   }
	   i = i +1; // next pano
	}
     }

    } 
    std::cout<<i<<std::endl;
  }
  
  parser.get_document()->write_to_file("a.xml");



}
/////////////////////////////
// Determine XML File Type //
/////////////////////////////

std::string XmlReader::checkXMLFileType(){

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
