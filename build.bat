@echo off
if not exist "build" mkdir build
cd build
cmake -G "MSYS Makefiles" ^
    -DCMAKE_CXX_COMPILER=g++ ^
    -DCMAKE_C_COMPILER=gcc ^
    -DCMAKE_MAKE_PROGRAM=make ^
    ..
if errorlevel 1 (
    echo CMake configuration failed
    cd ..
    exit /b 1
)
cmake --build .
if errorlevel 1 (
    echo Build failed
    cd ..
    exit /b 1
)
cd ..