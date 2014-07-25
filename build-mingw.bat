@echo off
set debug=1

ld -r -b binary -o obj/shaders/vs.o src/shaders/vs.c
ld -r -b binary -o obj/shaders/fs.o src/shaders/fs.c
g++ -c -I"glfw-3.0.4.bin.WIN32\include" -o obj/hr.o src/hr.cpp

if %debug% equ 1 (
	g++ -o bin/hr.exe obj/hr.o obj/shaders/vs.o obj/shaders/fs.o -I"glfw-3.0.4.bin.WIN32\include" -L"glfw-3.0.4.bin.WIN32\lib-mingw" -lglfw3 -lgdi32 -lglu32 -lopengl32
) else (
	g++ -o bin/hr.exe obj/hr.o obj/shaders/vs.o obj/shaders/fs.o -I"glfw-3.0.4.bin.WIN32\include" -L"glfw-3.0.4.bin.WIN32\lib-mingw" -lglfw3 -lgdi32 -lglu32 -lopengl32 -mwindows
)

