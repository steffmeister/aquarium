# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/steff/mystuff/devel/aquarium

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/steff/mystuff/devel/aquarium

# Include any dependencies generated for this target.
include src/CMakeFiles/../aquarium.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/../aquarium.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/../aquarium.dir/flags.make

src/CMakeFiles/../aquarium.dir/main.c.o: src/CMakeFiles/../aquarium.dir/flags.make
src/CMakeFiles/../aquarium.dir/main.c.o: src/main.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/steff/mystuff/devel/aquarium/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/../aquarium.dir/main.c.o"
	cd /home/steff/mystuff/devel/aquarium/src && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/../aquarium.dir/main.c.o   -c /home/steff/mystuff/devel/aquarium/src/main.c

src/CMakeFiles/../aquarium.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/../aquarium.dir/main.c.i"
	cd /home/steff/mystuff/devel/aquarium/src && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/steff/mystuff/devel/aquarium/src/main.c > CMakeFiles/../aquarium.dir/main.c.i

src/CMakeFiles/../aquarium.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/../aquarium.dir/main.c.s"
	cd /home/steff/mystuff/devel/aquarium/src && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/steff/mystuff/devel/aquarium/src/main.c -o CMakeFiles/../aquarium.dir/main.c.s

src/CMakeFiles/../aquarium.dir/main.c.o.requires:
.PHONY : src/CMakeFiles/../aquarium.dir/main.c.o.requires

src/CMakeFiles/../aquarium.dir/main.c.o.provides: src/CMakeFiles/../aquarium.dir/main.c.o.requires
	$(MAKE) -f src/CMakeFiles/../aquarium.dir/build.make src/CMakeFiles/../aquarium.dir/main.c.o.provides.build
.PHONY : src/CMakeFiles/../aquarium.dir/main.c.o.provides

src/CMakeFiles/../aquarium.dir/main.c.o.provides.build: src/CMakeFiles/../aquarium.dir/main.c.o
.PHONY : src/CMakeFiles/../aquarium.dir/main.c.o.provides.build

# Object files for target ../aquarium
__/aquarium_OBJECTS = \
"CMakeFiles/../aquarium.dir/main.c.o"

# External object files for target ../aquarium
__/aquarium_EXTERNAL_OBJECTS =

src/../aquarium: src/CMakeFiles/../aquarium.dir/main.c.o
src/../aquarium: /usr/lib/libSDLmain.a
src/../aquarium: /usr/lib/libSDL.so
src/../aquarium: /usr/lib/libSDL_image.so
src/../aquarium: /usr/lib/libSDL_mixer.so
src/../aquarium: /usr/lib/libSDL_ttf.so
src/../aquarium: src/CMakeFiles/../aquarium.dir/build.make
src/../aquarium: src/CMakeFiles/../aquarium.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable ../aquarium"
	cd /home/steff/mystuff/devel/aquarium/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/../aquarium.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/../aquarium.dir/build: src/../aquarium
.PHONY : src/CMakeFiles/../aquarium.dir/build

src/CMakeFiles/../aquarium.dir/requires: src/CMakeFiles/../aquarium.dir/main.c.o.requires
.PHONY : src/CMakeFiles/../aquarium.dir/requires

src/CMakeFiles/../aquarium.dir/clean:
	cd /home/steff/mystuff/devel/aquarium/src && $(CMAKE_COMMAND) -P CMakeFiles/../aquarium.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/../aquarium.dir/clean

src/CMakeFiles/../aquarium.dir/depend:
	cd /home/steff/mystuff/devel/aquarium && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/steff/mystuff/devel/aquarium /home/steff/mystuff/devel/aquarium/src /home/steff/mystuff/devel/aquarium /home/steff/mystuff/devel/aquarium/src /home/steff/mystuff/devel/aquarium/src/aquarium.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/../aquarium.dir/depend

