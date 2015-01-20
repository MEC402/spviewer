spViewer is a simple spherical panoramic viewer.
It can load large sets of stereo images and displays
them using OpenSceneGraph and OpenGL.

It works in generaly on any platform OpenSceneGraph is on.

Command Line Options
--------------------

  spViewer --menu {menufile.html} --xml {panormafiles.xml}

  Flags:
  
    --menu => Specified HTML Menu File
    --xml  => Panorama XML File 

  Example: 
  
    ./spViewer -xin --menu a.html --xml xmls/panos_4k.xmls
  
    
Keyboard Controls
-----------------

  VIEWING:
  - w: Rotate up
  - s: Rotate down
  - a: Rotate left
  - d: Rotate right
  - q: Decrease Vertical FOV/Zoom In
  - z: Increase Vertical FOV/Zoom In
  - [: Increase Horizontal FOV
  - ]: Decrease Horizontal FOV
  - e: Reset View
  
  STEREO:
  - p/P: Rotate Left Eye
  - t: Swap Left/Right Images

  MENU:
  - m: Toggle menu visibility
  
  DEBUG: 
  - R: Toggle Reference Grid
  - I: RAM usage
  
  EXPERIMENTAL
  - g: display the degree of rotation of the left sphere and write it to file
  - G: turn off the display of degree of left sphere's rotation
  - K/k: move the camera to the left/right
  - L/l: move the camera back/forward
  
  
  UNASSIGNED:
  - 1: Increase Eye Distance	(eyeseparation.cpp)
  - 2: Decrease Eye Distance	(eyeseparation.cpp)
  - 3: Toggle Stereo Panoramas	(eyeseparation.cpp)
  - v: Left/Right Stereo Mode	(eyeseparation.cpp)
  - h: Top/Bottom Stereo Mode	(eyeseparation.cpp)
  - b: Anaglyphic Stereo Mode	(eyeseparation.cpp)
