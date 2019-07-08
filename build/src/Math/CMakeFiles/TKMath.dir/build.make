# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/sky/Github/vkTestProj

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sky/Github/vkTestProj/build

# Include any dependencies generated for this target.
include src/Math/CMakeFiles/TKMath.dir/depend.make

# Include the progress variables for this target.
include src/Math/CMakeFiles/TKMath.dir/progress.make

# Include the compile flags for this target's objects.
include src/Math/CMakeFiles/TKMath.dir/flags.make

src/Math/CMakeFiles/TKMath.dir/TKCamera.cpp.o: src/Math/CMakeFiles/TKMath.dir/flags.make
src/Math/CMakeFiles/TKMath.dir/TKCamera.cpp.o: ../src/Math/TKCamera.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sky/Github/vkTestProj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/Math/CMakeFiles/TKMath.dir/TKCamera.cpp.o"
	cd /home/sky/Github/vkTestProj/build/src/Math && /usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TKMath.dir/TKCamera.cpp.o -c /home/sky/Github/vkTestProj/src/Math/TKCamera.cpp

src/Math/CMakeFiles/TKMath.dir/TKCamera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TKMath.dir/TKCamera.cpp.i"
	cd /home/sky/Github/vkTestProj/build/src/Math && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sky/Github/vkTestProj/src/Math/TKCamera.cpp > CMakeFiles/TKMath.dir/TKCamera.cpp.i

src/Math/CMakeFiles/TKMath.dir/TKCamera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TKMath.dir/TKCamera.cpp.s"
	cd /home/sky/Github/vkTestProj/build/src/Math && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sky/Github/vkTestProj/src/Math/TKCamera.cpp -o CMakeFiles/TKMath.dir/TKCamera.cpp.s

src/Math/CMakeFiles/TKMath.dir/TKComplex.cpp.o: src/Math/CMakeFiles/TKMath.dir/flags.make
src/Math/CMakeFiles/TKMath.dir/TKComplex.cpp.o: ../src/Math/TKComplex.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sky/Github/vkTestProj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/Math/CMakeFiles/TKMath.dir/TKComplex.cpp.o"
	cd /home/sky/Github/vkTestProj/build/src/Math && /usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TKMath.dir/TKComplex.cpp.o -c /home/sky/Github/vkTestProj/src/Math/TKComplex.cpp

src/Math/CMakeFiles/TKMath.dir/TKComplex.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TKMath.dir/TKComplex.cpp.i"
	cd /home/sky/Github/vkTestProj/build/src/Math && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sky/Github/vkTestProj/src/Math/TKComplex.cpp > CMakeFiles/TKMath.dir/TKComplex.cpp.i

src/Math/CMakeFiles/TKMath.dir/TKComplex.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TKMath.dir/TKComplex.cpp.s"
	cd /home/sky/Github/vkTestProj/build/src/Math && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sky/Github/vkTestProj/src/Math/TKComplex.cpp -o CMakeFiles/TKMath.dir/TKComplex.cpp.s

src/Math/CMakeFiles/TKMath.dir/TKDataType.cpp.o: src/Math/CMakeFiles/TKMath.dir/flags.make
src/Math/CMakeFiles/TKMath.dir/TKDataType.cpp.o: ../src/Math/TKDataType.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sky/Github/vkTestProj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/Math/CMakeFiles/TKMath.dir/TKDataType.cpp.o"
	cd /home/sky/Github/vkTestProj/build/src/Math && /usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TKMath.dir/TKDataType.cpp.o -c /home/sky/Github/vkTestProj/src/Math/TKDataType.cpp

src/Math/CMakeFiles/TKMath.dir/TKDataType.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TKMath.dir/TKDataType.cpp.i"
	cd /home/sky/Github/vkTestProj/build/src/Math && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sky/Github/vkTestProj/src/Math/TKDataType.cpp > CMakeFiles/TKMath.dir/TKDataType.cpp.i

src/Math/CMakeFiles/TKMath.dir/TKDataType.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TKMath.dir/TKDataType.cpp.s"
	cd /home/sky/Github/vkTestProj/build/src/Math && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sky/Github/vkTestProj/src/Math/TKDataType.cpp -o CMakeFiles/TKMath.dir/TKDataType.cpp.s

src/Math/CMakeFiles/TKMath.dir/TKMatrix.cpp.o: src/Math/CMakeFiles/TKMath.dir/flags.make
src/Math/CMakeFiles/TKMath.dir/TKMatrix.cpp.o: ../src/Math/TKMatrix.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sky/Github/vkTestProj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/Math/CMakeFiles/TKMath.dir/TKMatrix.cpp.o"
	cd /home/sky/Github/vkTestProj/build/src/Math && /usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TKMath.dir/TKMatrix.cpp.o -c /home/sky/Github/vkTestProj/src/Math/TKMatrix.cpp

src/Math/CMakeFiles/TKMath.dir/TKMatrix.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TKMath.dir/TKMatrix.cpp.i"
	cd /home/sky/Github/vkTestProj/build/src/Math && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sky/Github/vkTestProj/src/Math/TKMatrix.cpp > CMakeFiles/TKMath.dir/TKMatrix.cpp.i

src/Math/CMakeFiles/TKMath.dir/TKMatrix.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TKMath.dir/TKMatrix.cpp.s"
	cd /home/sky/Github/vkTestProj/build/src/Math && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sky/Github/vkTestProj/src/Math/TKMatrix.cpp -o CMakeFiles/TKMath.dir/TKMatrix.cpp.s

src/Math/CMakeFiles/TKMath.dir/TKVector.cpp.o: src/Math/CMakeFiles/TKMath.dir/flags.make
src/Math/CMakeFiles/TKMath.dir/TKVector.cpp.o: ../src/Math/TKVector.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sky/Github/vkTestProj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/Math/CMakeFiles/TKMath.dir/TKVector.cpp.o"
	cd /home/sky/Github/vkTestProj/build/src/Math && /usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TKMath.dir/TKVector.cpp.o -c /home/sky/Github/vkTestProj/src/Math/TKVector.cpp

src/Math/CMakeFiles/TKMath.dir/TKVector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TKMath.dir/TKVector.cpp.i"
	cd /home/sky/Github/vkTestProj/build/src/Math && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sky/Github/vkTestProj/src/Math/TKVector.cpp > CMakeFiles/TKMath.dir/TKVector.cpp.i

src/Math/CMakeFiles/TKMath.dir/TKVector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TKMath.dir/TKVector.cpp.s"
	cd /home/sky/Github/vkTestProj/build/src/Math && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sky/Github/vkTestProj/src/Math/TKVector.cpp -o CMakeFiles/TKMath.dir/TKVector.cpp.s

# Object files for target TKMath
TKMath_OBJECTS = \
"CMakeFiles/TKMath.dir/TKCamera.cpp.o" \
"CMakeFiles/TKMath.dir/TKComplex.cpp.o" \
"CMakeFiles/TKMath.dir/TKDataType.cpp.o" \
"CMakeFiles/TKMath.dir/TKMatrix.cpp.o" \
"CMakeFiles/TKMath.dir/TKVector.cpp.o"

# External object files for target TKMath
TKMath_EXTERNAL_OBJECTS =

src/Math/libTKMath.a: src/Math/CMakeFiles/TKMath.dir/TKCamera.cpp.o
src/Math/libTKMath.a: src/Math/CMakeFiles/TKMath.dir/TKComplex.cpp.o
src/Math/libTKMath.a: src/Math/CMakeFiles/TKMath.dir/TKDataType.cpp.o
src/Math/libTKMath.a: src/Math/CMakeFiles/TKMath.dir/TKMatrix.cpp.o
src/Math/libTKMath.a: src/Math/CMakeFiles/TKMath.dir/TKVector.cpp.o
src/Math/libTKMath.a: src/Math/CMakeFiles/TKMath.dir/build.make
src/Math/libTKMath.a: src/Math/CMakeFiles/TKMath.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sky/Github/vkTestProj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX static library libTKMath.a"
	cd /home/sky/Github/vkTestProj/build/src/Math && $(CMAKE_COMMAND) -P CMakeFiles/TKMath.dir/cmake_clean_target.cmake
	cd /home/sky/Github/vkTestProj/build/src/Math && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TKMath.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/Math/CMakeFiles/TKMath.dir/build: src/Math/libTKMath.a

.PHONY : src/Math/CMakeFiles/TKMath.dir/build

src/Math/CMakeFiles/TKMath.dir/clean:
	cd /home/sky/Github/vkTestProj/build/src/Math && $(CMAKE_COMMAND) -P CMakeFiles/TKMath.dir/cmake_clean.cmake
.PHONY : src/Math/CMakeFiles/TKMath.dir/clean

src/Math/CMakeFiles/TKMath.dir/depend:
	cd /home/sky/Github/vkTestProj/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sky/Github/vkTestProj /home/sky/Github/vkTestProj/src/Math /home/sky/Github/vkTestProj/build /home/sky/Github/vkTestProj/build/src/Math /home/sky/Github/vkTestProj/build/src/Math/CMakeFiles/TKMath.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/Math/CMakeFiles/TKMath.dir/depend

