 
#ifndef XMLREADER_H_
#define XMLREADER_H_ 

#include <libxml++/libxml++.h>
#include "Control.h"
#include "Panorama.h"


class XmlReader { 
  
public:
  
  XmlReader();
  ~XmlReader();
  
  void debug(const xmlpp::Node* node);
  std::string checkXMLFileType(const xmlpp::Node* node);

  void parseControls(const xmlpp::Node* node);
  void parsePanoramas(const xmlpp::Node* node);
  
  Controls getControls(){ return controlVec; }
  std::vector<Panorama*> getPanoramas(){ return panoVec; }
  
  std::string getXMLTYPECONTROL(){return XML_TYPE_CONTROLS;}
  std::string getXMLTYPEPANOS(){return XML_TYPE_PANOS;}

private:
  
  // String Constants (Make Constants?)
  std::string XML_TYPE_CONTROLS;
  std::string XML_TYPE_PANOS;
  
  Controls controlVec;
  std::vector<Panorama*> panoVec;
  
};

#endif