#
# Makefile for igl-hr
# This file is made for windows
#

CXXFLAGS=-g -Wall
INCLUDES=-I"glfw-3.0.4.bin.WIN32/include"
LINKLIBES=-L"glfw-3.0.4.bin.WIN32/lib-mingw" -lglfw3 -lgdi32 -lglu32 -lopengl32

# the object files we need are
#  - one from every .cpp in src
#  - one from every .c in src/shaders
OBJECTS=$(patsubst src/%.cpp,obj/%.o,$(wildcard src/*.cpp)) \
        $(patsubst src/shaders/%.c,obj/shaders/%.o,$(wildcard src/shaders/*.c))

all: bin/hr.exe

# for linking to a windows executable without console in the background, add option -mwindows (where?)
bin/hr.exe: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o bin/hr.exe \
	                  $(OBJECTS) \
	                  $(INCLUDES) $(LINKLIBES)

# shaders are a bit special
obj/shaders/%.o: src/shaders/%.c
	ld -r -b binary -o $@ $<
	objcopy --redefine-sym _binary_src_shaders_$*_c_start=_source_$*_start \
	        --redefine-sym _binary_src_shaders_$*_c_size=_source_$*_size \
	        --redefine-sym _binary_src_shaders_$*_c_end=_source_$*_end \
	        obj/shaders/$*.o

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	del /S obj\*.o
	del /S bin\*.exe