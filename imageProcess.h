

#ifndef _IMAGE_PROCESS_H_
#define _IMAGE_PROCESS_H_
#include <string>
#include <vector>
#include "XmlReader.h"
using namespace std;
class imageProcess: public XmlReader
{
protected:
 // static vector<double> rotatedLeftSphere;
  static int index;
public:
   // void setVector(vector<double> a);
    void setIndex(int a);
    int getIndex();

};

#endif
