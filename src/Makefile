INCLUDE = -I. -IC:/msys64/mingw64/bin/../include/SDL2 -Dmain=SDL_main\
-I..\thirdparty\glad-master\include \
-I..\thirdparty\imgui -I..\thirdparty\imgui\backends -I..\thirdparty\imgui\misc\cpp\
-IC:/msys64/mingw64/include/libpng16 -DHWY_SHARED_DEFINE \
-IC:/msys64/mingw64/include/webp -DLIBDEFLATE_DLL \
-I../thirdparty 

LIBS = -LC:/msys64/mingw64/bin/../lib -lSDL2_image -lmingw32 -lSDL2main -lSDL2
IMGUI_SRC = ../thirdparty/imgui/imgui.cpp ../thirdparty/imgui/imgui_demo.cpp \
../thirdparty/imgui/imgui_draw.cpp ../thirdparty/imgui/imgui_tables.cpp \
../thirdparty/imgui/imgui_widgets.cpp \
../thirdparty/imgui/backends/imgui_impl_opengl3.cpp \
../thirdparty/imgui/backends/imgui_impl_sdl2.cpp \
../thirdparty/imgui/misc/cpp/imgui_stdlib.cpp

all : ../test.exe
../test.exe : test.cpp ../thirdparty/glad-master/src/glad.c $(IMGUI_SRC)
	g++ $^ $(INCLUDE) $(LIBS) -o $@

clean :
	del ..\test.exe