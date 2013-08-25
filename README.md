Tetris3D

Tetris3D - a 3D version of an old popular Tetris game.

The game has been developed using C++ and Visual Studio 2010 on Microsoft Windows 7 Ultimate x64.

Basic libraries: SDL, OpenGL (GLU, GLUT), CML, FMOD.

The github repository is self-containing, it has all the necessary header/library files (except for OpenGL DLLs, which are assumed to be present on the machine). 

To run the game, just go to the Release directory and double click Tetris3D.exe.

UML class diagrams reflecting relationships of the most important classes can be found in Tetris3DclassUML.pdf.

_____________________
Notes about the game: 
1) in Tetris3D those rotations and movement of the figure that would allow figure's cubes to go outside the scene are not allowed;
2) "Lateral size" in Options means the number of cubes in the horizontal (i.e. x and z) directions;
3) a round lasts until the user fills in the whole scene or until the user annihilates about 15 layers;
4) in "Training" mode the figure doesn't fall automatically, the user has to force the figure to fall (using a SHIFT button).

___________________________________________________
(c) Mykola Prodanov, summer 2013, Juelich, Germany. 
This code is free to use and/or distribute. However, the code is provided 'as is', without any warranty that it doesn't contain bugs and will not lead to harmful effects to the software (e.g. OS) and/or hardware where the program is invoked. The author is not liable for such harmful effects if they happen as a result of using/running the code/program.