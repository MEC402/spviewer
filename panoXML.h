#ifndef _PANO_XML_H_
#define _PANO_XML_H_
#include "Panorama.h"

std::vector<Panorama*>  parsePanos(mxml_node_t *atree);
Panorama *parseSinglePano(mxml_node_t *anode);

#endif
