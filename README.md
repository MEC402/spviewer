how to make the reset button

=======================================
== Launch Parameters:                ==
=======================================

  Example: 
  
    ./spViewer -xin --menu a.html --xml xmls/panos_4k.xmls
  
  Flags:
  
    --menu => Specified HTML Menu File
    --xml  => Panorama XML File 
    
    -xin => Enable Xinerama screen support (Only use if using Xinerama)

    
    
    
=======================================
== Controls:                         ==
=======================================
  VIEWING:
  - w: Rotate up
  - s: Rotate down
  - a: Rotate left
  - d: Rotate right
  - q: Zoom in
  - z: Zoom out
  - [: Increase FOV
  - ]: Decrease FOV
  
  STEREO:
  - p/P: Rotate Left Eye
  - t: Swap Left/Right Images

  MENU:
  - m: Toggle menu visibility
  
  DEBUG: 
  - R: Toggle Reference Grid
  - I: RAM usage
  
  UNKNOWn:
  - e: reset
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
  
  
