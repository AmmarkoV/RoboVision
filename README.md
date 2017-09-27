![AmmarServer](https://raw.githubusercontent.com/AmmarkoV/RoboVision/master/Documentation/gddgbanner.png)

# RoboVision
## A GPLv3 Computer Vision / Robotics Project that aims to provide a stable compact software stack for generic vision enabled robotics!

##Documentation

To start , you can begin by reading my BSc thesis which while give you a detailed overview and maybe the computer vision theory that it is based on in the Documentation directory.  

https://github.com/AmmarkoV/RoboVision/blob/master/Documentation/GuarDDoG_RoboVision.pdf?raw=true

or 

https://docs.google.com/viewer?url=https%3A%2F%2Fgithub.com%2FAmmarkoV%2FRoboVision%2Fblob%2Fmaster%2FDocumentation%2FGuarDDoG_RoboVision.pdf%3Fraw%3Dtrue

https://github.com/AmmarkoV/RoboVision/tree/master/Documentation


##Status

Although it is under development several years now, it is still premature so I continue to work on it.
The testing platform for the software is my pet project named “GuardDoG” ( http://ammar.gr/gddg/ ) , a surveillance robot guard for homes and offices. Most of the layers of the project are abstract enough to be able to be used by different designs.


##Implementation

RoboVision works ONLY on linux, and uses V4L2, wxWidgets, OpenGL, Festival, CMU Sphinx, OpenCV , Apache and other foss as extensions.
I have made an effort to implement everything in a low enough level so that the console “daemon” mode will need only V4L2 support, and everything will be compiled together to reduce unneeded overheads.
Lately I have added OpenCV as a library dependency to speed up development by using some thoroughly tested function calls and reducing the debugging surface. 
An effort has been also made to keep libraries clean of bloat and the whole project could easily be ported to another platform , but there is currently no benefit for the extra maintenance cost , so there are no ( and probably will never be :P ) plans for porting it to Windows or Mac.   

Due to the complexity and combination of computer science fields involved ( Computer Vision , Natural Language Processing , Artificial Intelligence , Embedded Systems , Robotics [among the most important] ) the project has still not reached maturity and development is continued until an acceptable functionality level is reached..

Once you download the project ..

To automatically resolve the dependencies of the project run .Scripts/Setup/apt-get-guarddog-dependencies if you have a Debian/Ubuntu based distro ( package names may vary from version to version  , but I try to update them to the last possible version ) , or using .Scripts/Setup/pacman-guarddog-dependencies for Arch and pacman based distros.. 

To compile it you will need to run the custom make bash script in the parent directory typing ./make from the shell while being on the root directory of the project ( NOT "make" BUT "./make" it is not a standard makefile but a bash script  ) 

The IDE used for the development of the Project is  Code::Blocks , and you can use the workspaces on the root directory that open the sub projects , in any case , once you download a fresh copy of the project be sure to run ./make first to compile any libraries that may be needed and are not part of the workspace
 

I Hope you'll find this project/repository useful ..!
