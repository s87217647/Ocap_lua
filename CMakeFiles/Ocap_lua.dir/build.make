# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

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
CMAKE_COMMAND = /opt/local/bin/cmake

# The command to remove a file.
RM = /opt/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/swift/Lua_ocap/Ocap_lua

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/swift/Lua_ocap/Ocap_lua

# Include any dependencies generated for this target.
include CMakeFiles/Ocap_lua.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Ocap_lua.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Ocap_lua.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Ocap_lua.dir/flags.make

CMakeFiles/Ocap_lua.dir/onelua.c.o: CMakeFiles/Ocap_lua.dir/flags.make
CMakeFiles/Ocap_lua.dir/onelua.c.o: onelua.c
CMakeFiles/Ocap_lua.dir/onelua.c.o: CMakeFiles/Ocap_lua.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/swift/Lua_ocap/Ocap_lua/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Ocap_lua.dir/onelua.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Ocap_lua.dir/onelua.c.o -MF CMakeFiles/Ocap_lua.dir/onelua.c.o.d -o CMakeFiles/Ocap_lua.dir/onelua.c.o -c /Users/swift/Lua_ocap/Ocap_lua/onelua.c

CMakeFiles/Ocap_lua.dir/onelua.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Ocap_lua.dir/onelua.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/swift/Lua_ocap/Ocap_lua/onelua.c > CMakeFiles/Ocap_lua.dir/onelua.c.i

CMakeFiles/Ocap_lua.dir/onelua.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Ocap_lua.dir/onelua.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/swift/Lua_ocap/Ocap_lua/onelua.c -o CMakeFiles/Ocap_lua.dir/onelua.c.s

# Object files for target Ocap_lua
Ocap_lua_OBJECTS = \
"CMakeFiles/Ocap_lua.dir/onelua.c.o"

# External object files for target Ocap_lua
Ocap_lua_EXTERNAL_OBJECTS =

Ocap_lua: CMakeFiles/Ocap_lua.dir/onelua.c.o
Ocap_lua: CMakeFiles/Ocap_lua.dir/build.make
Ocap_lua: CMakeFiles/Ocap_lua.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/swift/Lua_ocap/Ocap_lua/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable Ocap_lua"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Ocap_lua.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Ocap_lua.dir/build: Ocap_lua
.PHONY : CMakeFiles/Ocap_lua.dir/build

CMakeFiles/Ocap_lua.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Ocap_lua.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Ocap_lua.dir/clean

CMakeFiles/Ocap_lua.dir/depend:
	cd /Users/swift/Lua_ocap/Ocap_lua && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/swift/Lua_ocap/Ocap_lua /Users/swift/Lua_ocap/Ocap_lua /Users/swift/Lua_ocap/Ocap_lua /Users/swift/Lua_ocap/Ocap_lua /Users/swift/Lua_ocap/Ocap_lua/CMakeFiles/Ocap_lua.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Ocap_lua.dir/depend

