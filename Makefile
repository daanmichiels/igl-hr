#
# Makefile for igl-hr
#

all: bin/hr.exe

# for linking to a windows executable without console in the background, add option -mwindows (where?)
bin/hr.exe: obj/hr.o \
			obj/shaders.o \
	        obj/shaders/vs.o \
	        obj/shaders/fs.o
	g++ -o bin/hr.exe obj/hr.o \
	                  obj/shaders.o \
	                  obj/shaders/vs.o \
	                  obj/shaders/fs.o \
	                  -I"glfw-3.0.4.bin.WIN32\include" -L"glfw-3.0.4.bin.WIN32\lib-mingw" -lglfw3 -lgdi32 -lglu32 -lopengl32

# shaders are a bit special
obj/shaders/%.o: src/shaders/%.c
	ld -r -b binary -o $@ $<
	objcopy --redefine-sym _binary_src_shaders_$*_c_start=_source_$*_start \
	        --redefine-sym _binary_src_shaders_$*_c_size=_source_$*_size \
	        --redefine-sym _binary_src_shaders_$*_c_end=_source_$*_end \
	        obj/shaders/$*.o

obj/shaders.o: src/shaders.cpp
	g++ -c -o obj/shaders.o src/shaders.cpp

obj/hr.o: src/hr.cpp
	g++ -g -c -I"glfw-3.0.4.bin.WIN32\include" -o obj/hr.o src/hr.cpp

