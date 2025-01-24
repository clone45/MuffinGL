@echo off
g++ -c src/main.cpp -I./include -Dmain=SDL_main
if errorlevel 1 exit /b 1
g++ -c src/graphics.cpp -I./include
if errorlevel 1 exit /b 1
g++ -c src/raster.cpp -I./include
if errorlevel 1 exit /b 1
g++ -c src/texture.cpp -I./include
if errorlevel 1 exit /b 1
g++ -c src/layer.cpp -I./include
if errorlevel 1 exit /b 1
g++ -c src/camera.cpp -I./include
if errorlevel 1 exit /b 1
g++ main.o graphics.o raster.o texture.o layer.o camera.o -o main.exe -L./lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
if errorlevel 1 exit /b 1
del main.o graphics.o raster.o texture.o layer.o camera.o