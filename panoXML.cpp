/*
 * parse panorama XML files
 *
 */

#include <mxml.h>
#include "panoXML.h"

std::vector<Panorama *> parsePanos(mxml_node_t *atree)
{
  std::vector<Panorama*> mypanos;
  Panorama *apano = NULL;
  mxml_node_t *node;
  for(node = mxmlFindElement(atree, atree,"pano",NULL, NULL,MXML_DESCEND_FIRST);
   node != NULL;
  node = mxmlFindElement(node, atree,"pano",NULL, NULL, MXML_NO_DESCEND)) 
  {
     apano = NULL;
     printf("Text: %s\n", node->value.element.name);
     apano = parseSinglePano(node); 
     if (apano != NULL) mypanos.push_back(apano);
  }
  
  return mypanos;
}
    
/*
 * parse out the attributes for a single panoram from an xml node
 */ 
Panorama * parseSinglePano(mxml_node_t *anode)
{
   const char * id;
   const char * name;
   const char * left;
   const char * right;
   const char * rotate;
   const char * vertical;
   const char * quat;

   id = mxmlElementGetAttr(anode,"id");
   name = mxmlElementGetAttr(anode,"name");
   left = mxmlElementGetAttr(anode,"left");
   right = mxmlElementGetAttr(anode,"right");
   rotate = mxmlElementGetAttr(anode,"rotate");
   vertical = mxmlElementGetAttr(anode,"vertical");
   quat = mxmlElementGetAttr(anode,"quat");

   std::string sid(id); 
   std::string sname(name); 
   std::string sleft(left); 
   std::string sright(right); 
   Panorama *res = new Panorama(sid,name,left,right);

   return res;;
}
