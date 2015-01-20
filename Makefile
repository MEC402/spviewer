#Makefile for spViewer
# simple stereo panoramic viewer with exposed controls and documentation.
# spViewer builds directly on top of OpenSceneGraph

#CXX = g++ 
CXXFLAGS = -std=c++11 -g -I/usr/include/ `pkg-config --cflags libxml++-2.6`
OSGLIBS = -losgViewer -losgGA -losgDB -losg -losgText 
XMLLIBS = -lawesomium-1-7 `pkg-config --libs libxml++-2.6`

LIBS=$(OSGLIBS) $(XMLLIBS)

SRCS = spViewer.cpp XmlReader.cpp tiles.cpp ivSphere.cpp\
          imageProcess.cpp Menu.cpp InteractiveHandler.cpp\
          AwesomiumImage.cpp MethodDispatcher.cpp eventListener.cpp\
          imageHandler.cpp spKeys.cpp MenuHandler.cpp eyeSeparation.cpp

OBJECTS = $(SRCS:.cpp=.o)


all: spViewer

spViewer:  $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o spViewer $(OBJECTS) $(LIBS)

.PHONY:
	
clean:
	rm -f spViewer *.o

