CC=g++
CPPFLAGS = -g -I/usr/include/ -losgViewer -losgGA -losgDB -losg -losgText -lXinerama -lawesomium-1-7 `pkg-config --cflags --libs libxml++-2.6`
OBJ= test.o XmlReader.o tiles.o

all:  $(OBJ) test
tiles.o: tiles.cpp tiles.h
test.o: test.cpp
XmlReader.o: XmlReader.cpp XmlReader.h
test: $(OBJ)
	$(CC) $(CPPFLAGS) $^ -o $@ 
clean:
	rm -f *.o test