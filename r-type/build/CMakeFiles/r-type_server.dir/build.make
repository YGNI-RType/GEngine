# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.30.3/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.30.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/basilefouquet/Dev/epitech/Tek3/advenced-c++/GameEngine/r-type

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/basilefouquet/Dev/epitech/Tek3/advenced-c++/GameEngine/r-type/build

# Include any dependencies generated for this target.
include CMakeFiles/r-type_server.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/r-type_server.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/r-type_server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/r-type_server.dir/flags.make

CMakeFiles/r-type_server.dir/sources/Start.cpp.o: CMakeFiles/r-type_server.dir/flags.make
CMakeFiles/r-type_server.dir/sources/Start.cpp.o: /Users/basilefouquet/Dev/epitech/Tek3/advenced-c++/GameEngine/r-type/sources/Start.cpp
CMakeFiles/r-type_server.dir/sources/Start.cpp.o: CMakeFiles/r-type_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/basilefouquet/Dev/epitech/Tek3/advenced-c++/GameEngine/r-type/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/r-type_server.dir/sources/Start.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/r-type_server.dir/sources/Start.cpp.o -MF CMakeFiles/r-type_server.dir/sources/Start.cpp.o.d -o CMakeFiles/r-type_server.dir/sources/Start.cpp.o -c /Users/basilefouquet/Dev/epitech/Tek3/advenced-c++/GameEngine/r-type/sources/Start.cpp

CMakeFiles/r-type_server.dir/sources/Start.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/r-type_server.dir/sources/Start.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/basilefouquet/Dev/epitech/Tek3/advenced-c++/GameEngine/r-type/sources/Start.cpp > CMakeFiles/r-type_server.dir/sources/Start.cpp.i

CMakeFiles/r-type_server.dir/sources/Start.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/r-type_server.dir/sources/Start.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/basilefouquet/Dev/epitech/Tek3/advenced-c++/GameEngine/r-type/sources/Start.cpp -o CMakeFiles/r-type_server.dir/sources/Start.cpp.s

CMakeFiles/r-type_server.dir/sources/main.cpp.o: CMakeFiles/r-type_server.dir/flags.make
CMakeFiles/r-type_server.dir/sources/main.cpp.o: /Users/basilefouquet/Dev/epitech/Tek3/advenced-c++/GameEngine/r-type/sources/main.cpp
CMakeFiles/r-type_server.dir/sources/main.cpp.o: CMakeFiles/r-type_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/basilefouquet/Dev/epitech/Tek3/advenced-c++/GameEngine/r-type/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/r-type_server.dir/sources/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/r-type_server.dir/sources/main.cpp.o -MF CMakeFiles/r-type_server.dir/sources/main.cpp.o.d -o CMakeFiles/r-type_server.dir/sources/main.cpp.o -c /Users/basilefouquet/Dev/epitech/Tek3/advenced-c++/GameEngine/r-type/sources/main.cpp

CMakeFiles/r-type_server.dir/sources/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/r-type_server.dir/sources/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/basilefouquet/Dev/epitech/Tek3/advenced-c++/GameEngine/r-type/sources/main.cpp > CMakeFiles/r-type_server.dir/sources/main.cpp.i

CMakeFiles/r-type_server.dir/sources/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/r-type_server.dir/sources/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/basilefouquet/Dev/epitech/Tek3/advenced-c++/GameEngine/r-type/sources/main.cpp -o CMakeFiles/r-type_server.dir/sources/main.cpp.s

# Object files for target r-type_server
r__type_server_OBJECTS = \
"CMakeFiles/r-type_server.dir/sources/Start.cpp.o" \
"CMakeFiles/r-type_server.dir/sources/main.cpp.o"

# External object files for target r-type_server
r__type_server_EXTERNAL_OBJECTS =

r-type_serverd: CMakeFiles/r-type_server.dir/sources/Start.cpp.o
r-type_serverd: CMakeFiles/r-type_server.dir/sources/main.cpp.o
r-type_serverd: CMakeFiles/r-type_server.dir/build.make
r-type_serverd: /Users/basilefouquet/Dev/epitech/Tek3/advenced-c++/GameEngine/build/libGEngine-sd.a
r-type_serverd: /opt/homebrew/Cellar/raylib/5.0/lib/libraylib.dylib
r-type_serverd: CMakeFiles/r-type_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/basilefouquet/Dev/epitech/Tek3/advenced-c++/GameEngine/r-type/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable r-type_serverd"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/r-type_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/r-type_server.dir/build: r-type_serverd
.PHONY : CMakeFiles/r-type_server.dir/build

CMakeFiles/r-type_server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/r-type_server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/r-type_server.dir/clean

CMakeFiles/r-type_server.dir/depend:
	cd /Users/basilefouquet/Dev/epitech/Tek3/advenced-c++/GameEngine/r-type/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/basilefouquet/Dev/epitech/Tek3/advenced-c++/GameEngine/r-type /Users/basilefouquet/Dev/epitech/Tek3/advenced-c++/GameEngine/r-type /Users/basilefouquet/Dev/epitech/Tek3/advenced-c++/GameEngine/r-type/build /Users/basilefouquet/Dev/epitech/Tek3/advenced-c++/GameEngine/r-type/build /Users/basilefouquet/Dev/epitech/Tek3/advenced-c++/GameEngine/r-type/build/CMakeFiles/r-type_server.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/r-type_server.dir/depend

