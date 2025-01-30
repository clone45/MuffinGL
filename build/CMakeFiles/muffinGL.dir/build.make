# CMAKE generated file: DO NOT EDIT!
# Generated by "MSYS Makefiles" Generator, CMake Version 3.26

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /C/msys64/mingw64/bin/cmake.exe

# The command to remove a file.
RM = /C/msys64/mingw64/bin/cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /C/Code/GameDev/muffinGL

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /C/Code/GameDev/muffinGL/build

# Include any dependencies generated for this target.
include CMakeFiles/muffinGL.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/muffinGL.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/muffinGL.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/muffinGL.dir/flags.make

CMakeFiles/muffinGL.dir/src/graphics.cpp.obj: CMakeFiles/muffinGL.dir/flags.make
CMakeFiles/muffinGL.dir/src/graphics.cpp.obj: C:/Code/GameDev/muffinGL/src/graphics.cpp
CMakeFiles/muffinGL.dir/src/graphics.cpp.obj: CMakeFiles/muffinGL.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/C/Code/GameDev/muffinGL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/muffinGL.dir/src/graphics.cpp.obj"
	/C/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/muffinGL.dir/src/graphics.cpp.obj -MF CMakeFiles/muffinGL.dir/src/graphics.cpp.obj.d -o CMakeFiles/muffinGL.dir/src/graphics.cpp.obj -c /C/Code/GameDev/muffinGL/src/graphics.cpp

CMakeFiles/muffinGL.dir/src/graphics.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/muffinGL.dir/src/graphics.cpp.i"
	/C/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /C/Code/GameDev/muffinGL/src/graphics.cpp > CMakeFiles/muffinGL.dir/src/graphics.cpp.i

CMakeFiles/muffinGL.dir/src/graphics.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/muffinGL.dir/src/graphics.cpp.s"
	/C/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /C/Code/GameDev/muffinGL/src/graphics.cpp -o CMakeFiles/muffinGL.dir/src/graphics.cpp.s

CMakeFiles/muffinGL.dir/src/raster.cpp.obj: CMakeFiles/muffinGL.dir/flags.make
CMakeFiles/muffinGL.dir/src/raster.cpp.obj: C:/Code/GameDev/muffinGL/src/raster.cpp
CMakeFiles/muffinGL.dir/src/raster.cpp.obj: CMakeFiles/muffinGL.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/C/Code/GameDev/muffinGL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/muffinGL.dir/src/raster.cpp.obj"
	/C/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/muffinGL.dir/src/raster.cpp.obj -MF CMakeFiles/muffinGL.dir/src/raster.cpp.obj.d -o CMakeFiles/muffinGL.dir/src/raster.cpp.obj -c /C/Code/GameDev/muffinGL/src/raster.cpp

CMakeFiles/muffinGL.dir/src/raster.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/muffinGL.dir/src/raster.cpp.i"
	/C/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /C/Code/GameDev/muffinGL/src/raster.cpp > CMakeFiles/muffinGL.dir/src/raster.cpp.i

CMakeFiles/muffinGL.dir/src/raster.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/muffinGL.dir/src/raster.cpp.s"
	/C/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /C/Code/GameDev/muffinGL/src/raster.cpp -o CMakeFiles/muffinGL.dir/src/raster.cpp.s

CMakeFiles/muffinGL.dir/src/texture.cpp.obj: CMakeFiles/muffinGL.dir/flags.make
CMakeFiles/muffinGL.dir/src/texture.cpp.obj: C:/Code/GameDev/muffinGL/src/texture.cpp
CMakeFiles/muffinGL.dir/src/texture.cpp.obj: CMakeFiles/muffinGL.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/C/Code/GameDev/muffinGL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/muffinGL.dir/src/texture.cpp.obj"
	/C/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/muffinGL.dir/src/texture.cpp.obj -MF CMakeFiles/muffinGL.dir/src/texture.cpp.obj.d -o CMakeFiles/muffinGL.dir/src/texture.cpp.obj -c /C/Code/GameDev/muffinGL/src/texture.cpp

CMakeFiles/muffinGL.dir/src/texture.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/muffinGL.dir/src/texture.cpp.i"
	/C/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /C/Code/GameDev/muffinGL/src/texture.cpp > CMakeFiles/muffinGL.dir/src/texture.cpp.i

CMakeFiles/muffinGL.dir/src/texture.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/muffinGL.dir/src/texture.cpp.s"
	/C/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /C/Code/GameDev/muffinGL/src/texture.cpp -o CMakeFiles/muffinGL.dir/src/texture.cpp.s

CMakeFiles/muffinGL.dir/src/layer.cpp.obj: CMakeFiles/muffinGL.dir/flags.make
CMakeFiles/muffinGL.dir/src/layer.cpp.obj: C:/Code/GameDev/muffinGL/src/layer.cpp
CMakeFiles/muffinGL.dir/src/layer.cpp.obj: CMakeFiles/muffinGL.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/C/Code/GameDev/muffinGL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/muffinGL.dir/src/layer.cpp.obj"
	/C/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/muffinGL.dir/src/layer.cpp.obj -MF CMakeFiles/muffinGL.dir/src/layer.cpp.obj.d -o CMakeFiles/muffinGL.dir/src/layer.cpp.obj -c /C/Code/GameDev/muffinGL/src/layer.cpp

CMakeFiles/muffinGL.dir/src/layer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/muffinGL.dir/src/layer.cpp.i"
	/C/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /C/Code/GameDev/muffinGL/src/layer.cpp > CMakeFiles/muffinGL.dir/src/layer.cpp.i

CMakeFiles/muffinGL.dir/src/layer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/muffinGL.dir/src/layer.cpp.s"
	/C/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /C/Code/GameDev/muffinGL/src/layer.cpp -o CMakeFiles/muffinGL.dir/src/layer.cpp.s

CMakeFiles/muffinGL.dir/src/camera.cpp.obj: CMakeFiles/muffinGL.dir/flags.make
CMakeFiles/muffinGL.dir/src/camera.cpp.obj: C:/Code/GameDev/muffinGL/src/camera.cpp
CMakeFiles/muffinGL.dir/src/camera.cpp.obj: CMakeFiles/muffinGL.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/C/Code/GameDev/muffinGL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/muffinGL.dir/src/camera.cpp.obj"
	/C/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/muffinGL.dir/src/camera.cpp.obj -MF CMakeFiles/muffinGL.dir/src/camera.cpp.obj.d -o CMakeFiles/muffinGL.dir/src/camera.cpp.obj -c /C/Code/GameDev/muffinGL/src/camera.cpp

CMakeFiles/muffinGL.dir/src/camera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/muffinGL.dir/src/camera.cpp.i"
	/C/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /C/Code/GameDev/muffinGL/src/camera.cpp > CMakeFiles/muffinGL.dir/src/camera.cpp.i

CMakeFiles/muffinGL.dir/src/camera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/muffinGL.dir/src/camera.cpp.s"
	/C/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /C/Code/GameDev/muffinGL/src/camera.cpp -o CMakeFiles/muffinGL.dir/src/camera.cpp.s

# Object files for target muffinGL
muffinGL_OBJECTS = \
"CMakeFiles/muffinGL.dir/src/graphics.cpp.obj" \
"CMakeFiles/muffinGL.dir/src/raster.cpp.obj" \
"CMakeFiles/muffinGL.dir/src/texture.cpp.obj" \
"CMakeFiles/muffinGL.dir/src/layer.cpp.obj" \
"CMakeFiles/muffinGL.dir/src/camera.cpp.obj"

# External object files for target muffinGL
muffinGL_EXTERNAL_OBJECTS =

libmuffinGL.a: CMakeFiles/muffinGL.dir/src/graphics.cpp.obj
libmuffinGL.a: CMakeFiles/muffinGL.dir/src/raster.cpp.obj
libmuffinGL.a: CMakeFiles/muffinGL.dir/src/texture.cpp.obj
libmuffinGL.a: CMakeFiles/muffinGL.dir/src/layer.cpp.obj
libmuffinGL.a: CMakeFiles/muffinGL.dir/src/camera.cpp.obj
libmuffinGL.a: CMakeFiles/muffinGL.dir/build.make
libmuffinGL.a: CMakeFiles/muffinGL.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/C/Code/GameDev/muffinGL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX static library libmuffinGL.a"
	$(CMAKE_COMMAND) -P CMakeFiles/muffinGL.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/muffinGL.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/muffinGL.dir/build: libmuffinGL.a
.PHONY : CMakeFiles/muffinGL.dir/build

CMakeFiles/muffinGL.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/muffinGL.dir/cmake_clean.cmake
.PHONY : CMakeFiles/muffinGL.dir/clean

CMakeFiles/muffinGL.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MSYS Makefiles" /C/Code/GameDev/muffinGL /C/Code/GameDev/muffinGL /C/Code/GameDev/muffinGL/build /C/Code/GameDev/muffinGL/build /C/Code/GameDev/muffinGL/build/CMakeFiles/muffinGL.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/muffinGL.dir/depend

