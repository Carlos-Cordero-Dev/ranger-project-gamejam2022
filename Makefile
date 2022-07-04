all:
	g++ -std=c++17 -mwindows main.cpp -o main -ID:\DevkitPro\msys2\mingw64\include\SDL2 -LD:\DevkitPro\msys2\mingw64\lib -Wall -lmingw32 -lSDL2main -lSDL2 -mconsole -lSDL2_Image -lSDL2_Mixer -lSDL2_ttf -lopengl32

# HACER -I y -L localmente (llevandote tu la carpeta SLD2)
# -I -----------------------------
# para incluir la libreria localmente (llevandote los includes a mano)D:\DevkitPro\examples\windows\custom-hello-world\SDL2\include
# este ultimo path lo he sacado de D:\Local SDL2\sdl\SDL2-2.0.20\x86_64-w64-mingw32 ,que he copiado y renombrado a SDL2 en el
# directorio actual
#
# -L -------------------------------
# la parte mingw64 tienes que hacerla desde donde este mingw64\lib
# la parte de sdl va con el archivo que has usado para -I, en la carpeta renombrada SLD2 ahora hay una carpeta /lib, esa es
# D:\DevkitPro\examples\windows\custom-hello-world\SDL2\lib
#
# -mwindows -mconsole para conseguir logs en windows