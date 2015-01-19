#Makefile for spViewer
# simple stereo panoramic viewer with exposed controls and documentation.
# spViewer builds directly on top of OpenSceneGraph
# All controls are exposed and highlighted for debugging purposes

CPP = g++ 
CPPFLAGS = -std=c++11 -g -I/usr/include/ -losgViewer -losgGA -losgDB -losg -losgText -lXinerama -lawesomium-1-7 `pkg-config --cflags --libs libxml++-2.6`
OBJECTS = spViewer.o XmlReader.o tiles.o ivSphere.o imageProcess.o Menu.o InteractiveHandler.o AwesomiumImage.o eventListener.o  imageHandler.o spKeys.o MenuHandler.o eyeSeparation.o


.SUFFIXES: .cpp .o
.cpp.o: $(CPP) $(CPPFLAGS) -c $<
.o: $(CPP) $(CPPFLAGS) $^ -o $@


all: main spViewer

main: $(OBJECTS)
spViewer.o: spViewer.cpp
tiles.o: tiles.cpp tiles.h
ivSphere.o: ivSphere.cpp ivSphere.h
imageProcess.o: imageProcess.cpp imageProcess.h
Menu.o: Menu.cpp Menu.h
InteractiveHandler.o: InteractiveHandler.cpp InteractiveHandler.h
AwesomiumImage.o: AwesomiumImage.cpp AwesomiumImage.h 
eventListener.o: eventListener.cpp eventListener.h
imageHandler.o: imageHandler.cpp imageHandler.h 
spKeys.o: spKeys.cpp spKeys.h
MenuHandler.o: MenuHandler.cpp MenuHandler.h
eyeSeparation.o: eyeSeparation.cpp eyeSeparation.h
XmlReader.o: XmlReader.cpp XmlReader.h
MethodDispatcher.o: MethodDispatcher.cpp MethodDispatcher.h 

.PHONY:
spViewer:  spViewer.o XmlReader.o tiles.o ivSphere.o imageProcess.o Menu.o InteractiveHandler.o AwesomiumImage.o eventListener.o  imageHandler.o spKeys.o MenuHandler.o eyeSeparation.o MethodDispatcher.o
	$(CPP) $(CPPFLAGS) -o spViewer spViewer.o XmlReader.o tiles.o ivSphere.o imageProcess.o Menu.o InteractiveHandler.o AwesomiumImage.o eventListener.o imageHandler.o MenuHandler.o spKeys.o eyeSeparation.o MethodDispatcher.o
	
clean:
	rm -f spViewer *.o
