#
# Makefile for igl-hr
# 
# This file assumes that you have prebuilt the third-party
# libraries, and that they can be found under thirparty/libraryname/lib.
# The library headers should be in thirdparty/libraryname/include.
#

CXXFLAGS=-g -Wall -std=c++11 -static-libgcc -Wl,-static
CXXFLAGS+= -DGLM_FORCE_RADIANS
INCLUDES=-I"thirdparty/glfw/include" -I"thirdparty/glew/include"

ifneq (,$(findstring MINGW,$(shell uname -s)))
        CXXFLAGS+= -DGLEW_STATIC
        LINKLIBES=-L"thirdparty/glfw/lib" -L"thirdparty/glew/lib" -lglew32 -lglfw3 -lgdi32 -lglu32 -lopengl32
        EXECUTABLE=bin/hyperbolic-space-on-oculus-rift.exe
else
	uname_s := $(shell uname -s)
ifeq ($(uname_s),Linux)
	LINKLIBES=-L/usr/lib -lglfw3 -pthread -lGLEW -lGLU -lGL -lrt -lXrandr -lXxf86vm -lXi -lXinerama -lX11  
  INCLUDES=-I/usr/include -I/usr/local/include
	CXXFLAGS = -g -Wall -std=c++11 -DGLM_FORCE_RADIANS
	EXECUTABLE=bin/HSOR
else
ifeq ($(uname_s),Darwin)
	CXX=clang++
	LINKLIBES=-L/usr/lib -lglfw3 -pthread -lGLEW 
  INCLUDES=-I/usr/include -I/usr/local/include
	CXXFLAGS = -g -Wall -std=c++11 -DGLM_FORCE_RADIANS -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
	EXECUTABLE=bin/HSOR
else
  $(error Unknown platform.)
endif
endif

endif

# the object files we need are
#  - one from every .cpp in src
#  - one from every .c in src/shaders
OBJECTS=$(patsubst src/%.cpp,obj/%.o,$(wildcard src/*.cpp))

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) \
	                  $(OBJECTS) \
	                  $(INCLUDES) $(LINKLIBES)

# shaders are a bit special
# note that these names are compiler-dependent, since
# different compiler mangle names differently
#obj/shaders/%.o: src/shaders/%.glsl
#	ld -r -b binary -o $@ $<
#	objcopy --redefine-sym _binary_src_shaders_$*_glsl_start=_source_$*_start \
#	        --redefine-sym _binary_src_shaders_$*_glsl_size=_source_$*_size \
#	        --redefine-sym _binary_src_shaders_$*_glsl_end=_source_$*_end \
#	        obj/shaders/$*.o

src/shaders/%.h: src/shaders/%.glsl
	python shaderwriter.py

obj/%.o: src/%.cpp $(patsubst src/shaders/%.glsl,src/shaders/%.h,$(wildcard src/shaders/*.glsl))
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

obj/object_converter/objconverter.o: src/object_converter/objconverter.cpp
	$(CXX) $(CXXFLAGS) -c -o obj/object_converter/objconverter.o src/object_converter/objconverter.cpp

objconverter: obj/object_converter/objconverter.o obj/hypermath.o
	$(CXX) $(CXXFLAGS) -o bin/objconverter \
										obj/object_converter/objconverter.o obj/hypermath.o

clean: 
	rm -f $(OBJECTS) obj/object_converter/objconverter.o
	rm -f $(EXECUTABLE) bin/objconverter

.PHONY: all clean

