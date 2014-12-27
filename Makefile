#
#  Makefile for Ray Tracer

# INCLUDE = -I/usr/include -I/usr/local/include -I/usr/local/include/FL/images  -O2 -march=x86-64 -fmessage-length=0 -Wall -I/usr/X11R6/include
INCLUDE = -I/usr/local/include -I/usr/local/include/FL/images -O2 -march=x86-64 -fmessage-length=0 -Wall -I/usr/X11R6/include

LIBDIR = -L/usr/local/lib -L/usr/X11R6/lib -Wl,-rpath 

LIBS  = `fltk-config --cxxflags --ldflags --use-gl --use-images --use-glut` -lm -lXext -lX11 -lm -lGL -lGLU

# LIBS = -lfltk -lfltk_gl -lXext -lX11 -lm -lGL -lGLU

CFLAGS = -g

CC = g++

.SUFFIXES: .o .cpp .cxx

.cpp.o: 
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $*.o $<

.cxx.o: 
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $*.o $<

ALL.O = src/main.o src/getopt.o src/RayTracer.o \
	src/ui/CommandLineUI.o src/ui/GraphicalUI.o src/ui/TraceGLWindow.o \
	src/ui/debuggingView.o src/ui/glObjects.o src/ui/debuggingWindow.o \
	src/ui/ModelerCamera.o \
	src/fileio/bitmap.o src/fileio/buffer.o \
	src/parser/Token.o src/parser/Tokenizer.o \
	src/parser/Parser.o src/parser/ParserException.o \
	src/scene/camera.o src/scene/light.o \
	src/scene/material.o src/scene/ray.o src/scene/scene.o \
	src/SceneObjects/Box.o src/SceneObjects/Cone.o \
	src/SceneObjects/Cylinder.o src/SceneObjects/trimesh.o \
	src/SceneObjects/Sphere.o src/SceneObjects/Square.o \
	src/accel/kdtree.o

ray: $(ALL.O)
	$(CC) $(CFLAGS) -o $@ $(ALL.O) $(INCLUDE) $(LIBDIR) $(LIBS)

clean:
	rm -f $(ALL.O)
