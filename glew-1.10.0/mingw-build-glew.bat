::
:: This file is based on http://stackoverflow.com/questions/6005076/building-glew-on-windows-with-mingw
::
:: I used it to build the mingw binaries of the glew library (1.10.0).
:: Download the source of glew, put this batch file in the glew-1.10.0 folder
:: and run it. You can find the built library in 'lib'.
::


gcc -DGLEW_NO_GLU -O2 -Wall -W -Iinclude  -DGLEW_STATIC -o src/glew.o -c src/glew.c
::gcc -shared -Wl,-soname,libglew32.dll -Wl,--out-implib,lib/libglew32.dll.a    -o lib/glew32.dll src/glew.o -L/mingw/lib -lglu32 -lopengl32 -lgdi32 -luser32 -lkernel32
ar cr lib/libglew32.a src/glew.o
