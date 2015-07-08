/*
 * parse panorama XML files
 *
 */

#include <mxml.h>
#include "panoXML.h"
#include <osg/Quat>

std::vector<Panorama *> parsePanos(mxml_node_t *atree) {
	std::vector<Panorama*> mypanos;
	Panorama *apano = NULL;
	mxml_node_t *node;

	for (node = mxmlFindElement(atree, atree, "pano", NULL, NULL, MXML_DESCEND_FIRST);
		node != NULL;
		node = mxmlFindElement(node, atree,"pano",NULL, NULL, MXML_NO_DESCEND)) {
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
    const char * geometry;
    bool leftPano = false;
    bool rightPano = false;
 
    id = mxmlElementGetAttr(anode,"id");
    name = mxmlElementGetAttr(anode,"name");
    left = mxmlElementGetAttr(anode,"left");
    right = mxmlElementGetAttr(anode,"right");
    rotate = mxmlElementGetAttr(anode,"rotate");
    vertical = mxmlElementGetAttr(anode,"vertical");
    quat = mxmlElementGetAttr(anode,"quat");
    geometry = mxmlElementGetAttr(anode, "geometry");
 
    if (left) leftPano = true;
    if (right) rightPano = true;
 
    std::string sid; 
    std::string sname; 
    std::string sleft; 
    std::string sright;
    std::string squat;
	std::string sgeom;
 
    if (id != NULL) sid = id;
	else {
 	   std::cout << "Missing pano id. Exiting.";
 	   return 0;
    }

    if (name != NULL) sname = name;
	else {
		std::cout << "Missing pano name. Exiting.";
	   return 0;
    }
   
    if (leftPano && !rightPano) {
  		sleft = left;
 	    sright = left;
    } else if (!leftPano && rightPano) {
   	    sleft = right;
 	    sright = right;
    } else if (leftPano && rightPano) {
 	    sleft = left;
 	    sright = right;
    } else {
 	    std::cout << "No panoramas given. Exiting.";
 	    return 0;
    }
   
   if(quat != NULL) squat = quat;
   else squat = "0.0;0.0;0.0;0.1";

   if (geometry != NULL) sgeom = geometry; 
   else sgeom = "";

   Panorama *res = new Panorama(sid,name,sleft,sright,squat,sgeom);

   return res;
}

//TODO: Add method to panorama called get quat to string
void writeToFile( std::string fileName, std::vector<Panorama*> plist )
{

	mxml_node_t *xml; //parent node
	mxml_node_t *pano;
	mxml_node_t *data;
	FILE *fp;
	std::stringstream q;

	xml = mxmlNewXML("panoTest");
	data = mxmlNewElement(xml, "panoramas");
	
	for (int i = 0; i < plist.size(); i++) 
	{
		Panorama* ap = plist[i];
		q.str("");
		q << ap->getQuat().x() << ";";
		q << ap->getQuat().y() << ";";
		q << ap->getQuat().z() << ";";
		q << ap->getQuat().w();

		pano = mxmlNewElement(data, "pano");

		if (ap->getID().c_str() != NULL){
			mxmlElementSetAttr(pano, " id", ap->getID().c_str()); //Parameters: (node element is attached to, name of value, actual value)  
		}
		else{
			mxmlElementSetAttr(pano, " id", "ERROR");
		}

		if (ap->getName().c_str() != NULL){
			mxmlElementSetAttr(pano, "name", ap->getName().c_str());
		}
		else{
			mxmlElementSetAttr(pano, "name", "ERROR");
		}

		if (ap->getLeftDir().c_str() != NULL){
			mxmlElementSetAttr(pano, "left", ap->getLeftDir().c_str());
		}
		else{
			mxmlElementSetAttr(pano, "left", "ERROR");
		}

		if (ap->getRightDir().c_str() != NULL){
			mxmlElementSetAttr(pano, "right", ap->getRightDir().c_str());
		}
		else{
			mxmlElementSetAttr(pano, "right", "ERROR");
		}

		if (q.str().c_str() != NULL){
			mxmlElementSetAttr(pano, "quat", q.str().c_str());
		}
		else{
			mxmlElementSetAttr(pano, "quat", "0.0;0.0;0.0;0.1");
		}

		if (ap->getGeom().c_str() != NULL){
			mxmlElementSetAttr(pano, "geometry", ap->getGeom().c_str());
		}
		else{
			return;
		}
	}
		//filename.c_str() to replace panoTest.xml in fp
		fp = fopen("panoTest.xml", "w"); // parameters: 
		mxmlSaveFile(xml, fp, MXML_NO_CALLBACK);//(xml node tree to save, file to write to, whitespace callback)
		fclose(fp);
}


// TODO: Method for formating pano
/** void formatPanoXML(std::string filename) {


} */