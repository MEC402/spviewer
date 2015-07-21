#include "ivSphere.h"
#include <iostream>
#include <osg/Array>
//#include <math.h>

#define M_PI 3.14159265358

ivSphere::ivSphere(float radius, unsigned int rings, unsigned int sectors) : osg::Geometry() {
	sphereVertices = new osg::Vec3Array;
    sphereNormals = new osg::Vec3Array;
    sphereTexCoords = new osg::Vec2Array;
    float const R = 1./(float)(rings-1);
    float const S = 1./(float)(sectors-1);
    unsigned int r, s;
	// sphereGeode.addDrawable( &sphereGeometry );

	// Establish texture coordinates, vertex list, and normals
	for (r = 0; r < rings; r++) {

		for (s = 0; s < sectors; s++) {
			// float const y = sin( -M_PI + M_PI * r * R );
			// float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
			// float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );
			// vertical angle FOV=37, start at 71.5 degrees go 37 deg
			//double phi = 58/180.0*M_PI + 66.0/180.0*M_PI * (rings -r ) * R;
			double phi = 180.0 / 180.0*M_PI * (rings - r) * R;
			// horizontal FOV=66 start at 57 degrees go 66 degrees.
			// double theta = 32/180.0 * M_PI + 110/180.0*M_PI * s * S;
			double theta = 360.0 / 180.0*M_PI * s * S;
			float  y = -cos(phi);
			float  x = -cos(theta) * sin(phi);
			float  z = -sin(theta) * sin(phi);
			// x=ceil(x*1000+1)/1000;
			// y=ceil(y*1000+1)/1000;
			// z=ceil(z*1000+1)/1000;
			sphereTexCoords->push_back(osg::Vec2(s*R, r*R));
			// have swapped y and z from original.
			sphereVertices->push_back(osg::Vec3(x * radius, z * radius, y * radius));
			sphereNormals->push_back(osg::Vec3(-x, z, y));
		}
	}

	this->setVertexArray(sphereVertices);
	this->setTexCoordArray(0, sphereTexCoords);
	this->setNormalArray(sphereNormals);
        
	// Generate quads for each face. 
	for(r = 0; r < rings-1; r++)
            
		for(s = 0; s < sectors-1; s++) {
			spherePrimitiveSets.push_back(new osg::DrawElementsUInt( osg::PrimitiveSet::QUADS, 0 ));
			osg::DrawElementsUInt *face = spherePrimitiveSets.back();
			// Corners of quads should be in CCW order.
			face->push_back( (r + 0) * sectors + (s + 0) );
			face->push_back( (r + 0) * sectors + (s + 1) );
			face->push_back( (r + 1) * sectors + (s + 1) );
			face->push_back( (r + 1) * sectors + (s + 0) );
            this->addPrimitiveSet( face );
        }
	}

ivSphere::ivSphere(float radius, float lastart, float lastop, float lostart, float lostop, int atess) {
	sphereVertices = new osg::Vec3Array;
	sphereNormals = new osg::Vec3Array;
	sphereTexCoords = new osg::Vec2Array;
	int r, s;
	int rings = 10;
	int  sectors = 10;
	tessel = atess;
	float ft = (float)(tessel - 1);

	// Establish texture coordinates, vertex list, and normals
	for (r = 0; r < tessel; r++) {

		for (s = 0; s < tessel; s++) {
			double phi = (((tessel - 1) - r) / ft*(lostop - lostart)*M_PI / 180.0 + lostart*M_PI / 180.0);
			// horizontal FOV=66 start at 57 degrees go 66 degrees.
			// phi=ceil(phi*1000)/1000;
			double theta = s / ft*(lastop - lastart)*M_PI / 180.0 + lastart*M_PI / 180.0;
			// std::cerr << phi << " " << theta << "\n";
			// theta=ceil(theta*1000)/1000;
			float const y = -cos(phi);
			float const x = -(cos(theta) * sin(phi));
			float const z = -sin(theta) * sin(phi);
			//std::cerr << x << " " << y<<" "<<z << "\n";
			float sval = ((float)s / (float)(tessel - 1));
			float rval = ((float)r / (float)(tessel - 1));
			sphereTexCoords->push_back(osg::Vec2(sval, rval));
			// have swapped y and z from original.
			sphereVertices->push_back(osg::Vec3(x * radius, z * radius, y * radius));
			sphereNormals->push_back(osg::Vec3(0 - x, 0 - z, 0 - y));
			// _normalArray->push_back  ( osg::Vec3(-x * radius, -z * radius, -y * radius) );
		}
	}

	this->setVertexArray(sphereVertices);
	this->setTexCoordArray(0, sphereTexCoords);
	this->setNormalArray(sphereNormals);
	//this->setNormalBinding( osg::Geometry::BIND_OVERALL );

	// Generate quads for each face.  
	for (r = 0; r < tessel - 1; r++) {

		for (s = 0; s < tessel - 1; s++) {
			spherePrimitiveSets.push_back(
			new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0));
			osg::DrawElementsUInt *face = spherePrimitiveSets.back();
			// Corners of quads should be in CCW order.
			face->push_back((r + 0) * tessel + (s + 0));
			face->push_back((r + 0) * tessel + (s + 1));
			face->push_back((r + 1) * tessel + (s + 1));
			face->push_back((r + 1) * tessel + (s + 0));
			this->addPrimitiveSet(face);
		}
	}
}
