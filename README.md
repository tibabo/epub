# Epub QML Reader

The project uses Qt5 and qmake. Be sure to use Qt5 or the project will not build.

## Build & Execution

You can either open the `epub.pro` file in QtCreator or use qmake to build the project.

To build it using qmake and Makefile use the following commands:

 bash
# Create a build folder
$ mkdir build

# Enter the build folder
$ cd build

# Generate a Makefile
$ qmake ..

# Build
$ make

# Finally execute the project
$ ./epub
