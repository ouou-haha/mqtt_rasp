# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/ou/Desktop/mqtt/test/asyn_sub

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ou/Desktop/mqtt/test/asyn_sub/build

# Include any dependencies generated for this target.
include CMakeFiles/asyn_subscribe.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/asyn_subscribe.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/asyn_subscribe.dir/flags.make

CMakeFiles/asyn_subscribe.dir/src/asyn_sub.c.o: CMakeFiles/asyn_subscribe.dir/flags.make
CMakeFiles/asyn_subscribe.dir/src/asyn_sub.c.o: ../src/asyn_sub.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ou/Desktop/mqtt/test/asyn_sub/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/asyn_subscribe.dir/src/asyn_sub.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/asyn_subscribe.dir/src/asyn_sub.c.o   -c /home/ou/Desktop/mqtt/test/asyn_sub/src/asyn_sub.c

CMakeFiles/asyn_subscribe.dir/src/asyn_sub.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/asyn_subscribe.dir/src/asyn_sub.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ou/Desktop/mqtt/test/asyn_sub/src/asyn_sub.c > CMakeFiles/asyn_subscribe.dir/src/asyn_sub.c.i

CMakeFiles/asyn_subscribe.dir/src/asyn_sub.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/asyn_subscribe.dir/src/asyn_sub.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ou/Desktop/mqtt/test/asyn_sub/src/asyn_sub.c -o CMakeFiles/asyn_subscribe.dir/src/asyn_sub.c.s

CMakeFiles/asyn_subscribe.dir/src/asyn_sub.c.o.requires:

.PHONY : CMakeFiles/asyn_subscribe.dir/src/asyn_sub.c.o.requires

CMakeFiles/asyn_subscribe.dir/src/asyn_sub.c.o.provides: CMakeFiles/asyn_subscribe.dir/src/asyn_sub.c.o.requires
	$(MAKE) -f CMakeFiles/asyn_subscribe.dir/build.make CMakeFiles/asyn_subscribe.dir/src/asyn_sub.c.o.provides.build
.PHONY : CMakeFiles/asyn_subscribe.dir/src/asyn_sub.c.o.provides

CMakeFiles/asyn_subscribe.dir/src/asyn_sub.c.o.provides.build: CMakeFiles/asyn_subscribe.dir/src/asyn_sub.c.o


# Object files for target asyn_subscribe
asyn_subscribe_OBJECTS = \
"CMakeFiles/asyn_subscribe.dir/src/asyn_sub.c.o"

# External object files for target asyn_subscribe
asyn_subscribe_EXTERNAL_OBJECTS =

asyn_subscribe: CMakeFiles/asyn_subscribe.dir/src/asyn_sub.c.o
asyn_subscribe: CMakeFiles/asyn_subscribe.dir/build.make
asyn_subscribe: CMakeFiles/asyn_subscribe.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ou/Desktop/mqtt/test/asyn_sub/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable asyn_subscribe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/asyn_subscribe.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/asyn_subscribe.dir/build: asyn_subscribe

.PHONY : CMakeFiles/asyn_subscribe.dir/build

CMakeFiles/asyn_subscribe.dir/requires: CMakeFiles/asyn_subscribe.dir/src/asyn_sub.c.o.requires

.PHONY : CMakeFiles/asyn_subscribe.dir/requires

CMakeFiles/asyn_subscribe.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/asyn_subscribe.dir/cmake_clean.cmake
.PHONY : CMakeFiles/asyn_subscribe.dir/clean

CMakeFiles/asyn_subscribe.dir/depend:
	cd /home/ou/Desktop/mqtt/test/asyn_sub/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ou/Desktop/mqtt/test/asyn_sub /home/ou/Desktop/mqtt/test/asyn_sub /home/ou/Desktop/mqtt/test/asyn_sub/build /home/ou/Desktop/mqtt/test/asyn_sub/build /home/ou/Desktop/mqtt/test/asyn_sub/build/CMakeFiles/asyn_subscribe.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/asyn_subscribe.dir/depend

