#
# Makefile for igl-hr
# This file is made for windows
#

CXXFLAGS=-g -Wall -DGLEW_STATIC -std=c++11 -static-libgcc -static-libstdc++ -Wl,-static
INCLUDES=-I"thirdparty/glfw-3.0.4.bin.WIN64/include" -I"thirdparty/glew-1.10.0/include"
LINKLIBES=-L"thirdparty/glew-1.10.0/lib" -L"thirdparty/glfw-3.0.4.bin.WIN64/lib-mingw" -lglew32 -lglfw3 -lgdi32 -lglu32 -lopengl32
EXECUTABLE=bin/hr.exe

# the object files we need are
#  - one from every .cpp in src
#  - one from every .c in src/shaders
OBJECTS=$(patsubst src/%.cpp,obj/%.o,$(wildcard src/*.cpp)) \
        $(patsubst src/shaders/%.c,obj/shaders/%.o,$(wildcard src/shaders/*.c))

all: $(EXECUTABLE)

# for linking to a windows executable without console in the background, add option -mwindows (where?)
bin/hr.exe: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) \
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
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLE)

