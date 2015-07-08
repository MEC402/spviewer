#ifndef _PANO_XML_H_
#define _PANO_XML_H_
#include "Panorama.h"
#include <mxml.h>
#include <string>
#include <sstream>

std::vector<Panorama*>  parsePanos(mxml_node_t *atree);
Panorama *parseSinglePano(mxml_node_t *anode);
void writeToFile(std::string fileName, std::vector<Panorama*> plist);
//void formatPanoXML(std::string fileName);

#endif
