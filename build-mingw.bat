@echo off
set debug=1

::partially link the vertex shader
ld -r -b binary -o obj/shaders/vs.o src/shaders/vs.c
::rename symbol in object file to prevent ultralong names in c++ code
objcopy --redefine-sym _binary_src_shaders_vs_c_start=_vs_start ^
        --redefine-sym _binary_src_shaders_vs_c_size=_vs_size ^
        --redefine-sym _binary_src_shaders_vs_c_end=_vs_end ^
        obj/shaders/vs.o

::partially link the fragment shader
ld -r -b binary -o obj/shaders/fs.o src/shaders/fs.c
::rename symbol in object file to prevent ultralong names in c++ code
objcopy --redefine-sym _binary_src_shaders_fs_c_start=_fs_start ^
        --redefine-sym _binary_src_shaders_fs_c_size=_fs_size ^
        --redefine-sym _binary_src_shaders_fs_c_end=_fs_end ^
        obj/shaders/fs.o

::compile the c++ source
g++ -c -I"glfw-3.0.4.bin.WIN32\include" -o obj/hr.o src/hr.cpp

::link
if %debug% equ 1 (
	g++ -o bin/hr.exe obj/hr.o obj/shaders/vs.o obj/shaders/fs.o -I"glfw-3.0.4.bin.WIN32\include" -L"glfw-3.0.4.bin.WIN32\lib-mingw" -lglfw3 -lgdi32 -lglu32 -lopengl32
) else (
	g++ -o bin/hr.exe obj/hr.o obj/shaders/vs.o obj/shaders/fs.o -I"glfw-3.0.4.bin.WIN32\include" -L"glfw-3.0.4.bin.WIN32\lib-mingw" -lglfw3 -lgdi32 -lglu32 -lopengl32 -mwindows
)

