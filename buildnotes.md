
build notes for putting together spViewer

spViewer builds using the following libraries:

OpenSceneGraph - this is the core viewer library and is fundamental.
--on the mac OpenSceneGraph is not compiled with libc++ but is compiled with stdlibc++
--this causes a conflict with libxml++

libxml++ - this is the xml parsing library.  
--on the mac this is compiled with c++11, this conflicts with OpenSceneGraph

Awesomium -- this is the HTML rendering engine for our menu
--this is totally broken and never worked.
--Want to completely disconnect it from the system.
--Currently is only available in 32 bit and conflicts with 64 bit Mac.

Plan for resolving Mac conflicts:

-4 xFix Mac Development environment: broken brew and Ports: No Brew
-3 xComplete a proper install of OpenSceneGraph on Mac.
-2 Pull modifieable version from git
-1 add documentation files to git repository
-0.5 Disable all builds of all codes, except main.
0 Document spViewer and break up into clear major function actions: make components switchable.
0.5 Enable one core feature at a time back into spViewer
1 Remove Awesomium menu components completely from spViewer
2 No, compile, change xml lib: Can OpenSceneGraph be compiled with c++11?  if so, switch.
3 Remove libxml++ and replace with an alternative library.
--a hardcode a basic menu and xml loading for spViewer.
--consolide libxml++ code into a single function
--replace with tinyxml

After testing and since OpenSceneGraph builds on Mac have decided to use MacPorts OpenSceneGraph and replace xml library as needed. Use tinyxml
Not going to use brew, use MacPorts


Using -std=c++11 --stdlib=libstdc++ has fixed string compability issues
