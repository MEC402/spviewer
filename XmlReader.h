 
#ifndef XMLREADER_H_
#define XMLREADER_H_ 

#include <libxml++/libxml++.h>
#include "Control.h"
#include "Panorama.h"


class XmlReader { 
  
public:
  
  XmlReader();
  ~XmlReader();
  
  void debug();
    void debug(const xmlpp::Node* node);
  std::string checkXMLFileType();
  void setPnode(const xmlpp::Node* node);
  void parseControls();
      void parseControls(const xmlpp::Node* node);

  void parsePanoramas();
    void parsePanoramas(const xmlpp::Node* node);
void writeToFile(std::string D, std::string myid,std::string degree, std::string degreeVer);
void writeToFile(xmlpp::Document * d,const xmlpp::Node* node,std::string myid,std::string degree, std::string degreeVer);
  

  Controls getControls(){ return controlVec; };
  std::vector<Panorama*> getPanoramas(){ return panoVec; };
  //not necessary
  std::vector<double> getDegree(){ return rotatedLeftSphere; }
  std::vector<double> getDegree2(){ return verticalRotation; }
  int getTotalImages(){return panoVec.size();}
  
  std::string getXMLTYPECONTROL(){return XML_TYPE_CONTROLS;}
  std::string getXMLTYPEPANOS(){return XML_TYPE_PANOS;}

protected:
  
  // String Constants (Make Constants?)
  std::string XML_TYPE_CONTROLS;
  std::string XML_TYPE_PANOS;
  
  Controls controlVec;
  static const xmlpp::Node mynode;
  static const xmlpp::Node* node;
  static std::vector<Panorama*> panoVec;
  static std::vector<double> rotatedLeftSphere;
  static std::vector<double> verticalRotation;
    static std::vector<osg::Quat> myQuat;

  static std::vector<std::string> Ids;

};

#endif