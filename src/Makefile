INCLUDE = -IC:/msys64/mingw64/bin/../include -IC:/msys64/mingw64/bin/../include/SDL2 -Dmain=SDL_main\
-I..\thirdparty\glad-master\include
LIBS = -LC:/msys64/mingw64/bin/../lib -lmingw32 -lSDL2main -lSDL2

all : ../main.exe
../main.exe : main.cpp ../thirdparty/glad-master/src/glad.c
	g++ $^ $(INCLUDE) $(LIBS) -o $@ 