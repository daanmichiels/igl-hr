#
# Makefile for igl-hr
# 
# This file assumes that you have prebuilt the third-party
# libraries, and that they can be found under thirparty/libraryname/lib.
# The library headers should be in thirdparty/libraryname/include.
#

CXXFLAGS=-g -Wall -DGLEW_STATIC -std=c++0x -static-libgcc -Wl,-static
INCLUDES=-I"thirdparty/glfw/include" -I"thirdparty/glew/include"
LINKLIBES=-L"thirdparty/glfw/lib" -L"thirdparty/glew/lib" -lglew32 -lglfw3 -lgdi32 -lglu32 -lopengl32
EXECUTABLE=bin/hr.exe

# the object files we need are
#  - one from every .cpp in src
#  - one from every .c in src/shaders
OBJECTS=$(patsubst src/%.cpp,obj/%.o,$(wildcard src/*.cpp)) \
        $(patsubst src/shaders/%.glsl,obj/shaders/%.o,$(wildcard src/shaders/*.glsl))

all: $(EXECUTABLE)

bin/hr.exe: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) \
	                  $(OBJECTS) \
	                  $(INCLUDES) $(LINKLIBES)

# shaders are a bit special
# note that these names are compiler-dependent, since
# different compiler mangle names differently
obj/shaders/%.o: src/shaders/%.glsl
	ld -r -b binary -o $@ $<
	objcopy --redefine-sym _binary_src_shaders_$*_glsl_start=_source_$*_start \
	        --redefine-sym _binary_src_shaders_$*_glsl_size=_source_$*_size \
	        --redefine-sym _binary_src_shaders_$*_glsl_end=_source_$*_end \
	        obj/shaders/$*.o

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

clean: 
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLE)
