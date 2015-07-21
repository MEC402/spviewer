
#include "Panorama.h"
#include <osgDB/ReadFile>
  
Panorama::Panorama(std::string id, std::string name, std::string left_dir, std::string right_dir, std::string aquat, std::string ageom){
	this->id = id;
	this->name = name;
	this->left_dir = left_dir;
	this->right_dir = right_dir;
	char *amyquat = new char[aquat.size() + 1];
	aquat.copy(amyquat, aquat.size());
	double a[4];
	int k = 0;
	char* c = strtok(amyquat, ";");

	while (c != NULL) {
		a[k] = atof(c);
		//  std::cerr<<a[k]<<"\n";
		c = strtok(NULL, ";");
		k++;
	}

	this->myquat.set(a[0], a[1], a[2], a[3]);
	std::cerr << "Allocate Tile Sets" << std::endl;    
	left  = new Tiles(left_dir,  512, 512, false);
	right = new Tiles(right_dir, 512, 512, false);
	std::cerr << "Create Left Tile" << std::endl;    
	left->createTiles();
	std::cerr << "Create Right Tile" << std::endl;    
	right->createTiles();
	leftImages = left->getAllTiles();
	rightImages = right->getAllTiles();
	// load any attached geometry files.
	mygeometry = NULL;
	geom = ageom;
	if (ageom.size() > 0) {
		std::cerr << "Has attached Geoemtry file - loading " << ageom << std::endl;
		// readNodeFile returns Node *; mygeometry is rer_ptr	
		mygeometry = osgDB::readNodeFile(ageom);
		std::cerr << mygeometry << std::endl;
	}
}
  
Panorama::~Panorama(){};
  
  