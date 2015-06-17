/*
 * a portion of a sphere or an entire sphere
 *
 *  With a set aside image texture for the sphere.
 *  spheres range from 0 to 360 in the lattitudinal axis.
 *                and from 0 to 180 in the longitudinal axis
 */
#ifndef _IVSPHERE_H_
#define _IVSPHERE_H_
#include <vector>
#include <osg/Array>

#include <osg/Geometry>

class ivSphere : public osg::Geometry
{
    protected:
      osg::Vec3Array *sphereVertices;
      osg::Vec3Array *sphereNormals;
      osg::Vec2Array *sphereTexCoords;
      osg::ref_ptr<osg::Image > myimage;
      
      std::vector<osg::DrawElementsUInt *> spherePrimitiveSets;
      int tessel;
   
    public:
      // build a full sphere
      ivSphere(float radius, unsigned int rings, unsigned int sectors);
      // build a portion of a sphere
 
      // In grid coordinates 
      // griddiv is the number of units the grid is divided by out of 360.0
      ivSphere(float radius, float lastart, float lastop, float lostart, float lostop,
               int atess);
      void setImageTexture( osg::Image *aimage);

    osg::Geometry  *getGeometry()  const { return (osg::Geometry *)this;  }
    osg::Vec3Array *getVertices()  const { return sphereVertices;  }
    osg::Vec3Array *getNormals()   const { return sphereNormals;   }
    osg::Vec2Array *getTexCoords() const { return sphereTexCoords; }


};
#endif
