# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_COMMAND = /usr/local/cmake-3.28.0-rc1-linux-x86_64/bin/cmake

# The command to remove a file.
RM = /usr/local/cmake-3.28.0-rc1-linux-x86_64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/database_connect_pool

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/database_connect_pool/build

# Include any dependencies generated for this target.
include CMakeFiles/database_connect_pool.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/database_connect_pool.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/database_connect_pool.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/database_connect_pool.dir/flags.make

CMakeFiles/database_connect_pool.dir/main.cpp.o: CMakeFiles/database_connect_pool.dir/flags.make
CMakeFiles/database_connect_pool.dir/main.cpp.o: /root/database_connect_pool/main.cpp
CMakeFiles/database_connect_pool.dir/main.cpp.o: CMakeFiles/database_connect_pool.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/root/database_connect_pool/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/database_connect_pool.dir/main.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/database_connect_pool.dir/main.cpp.o -MF CMakeFiles/database_connect_pool.dir/main.cpp.o.d -o CMakeFiles/database_connect_pool.dir/main.cpp.o -c /root/database_connect_pool/main.cpp

CMakeFiles/database_connect_pool.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/database_connect_pool.dir/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/database_connect_pool/main.cpp > CMakeFiles/database_connect_pool.dir/main.cpp.i

CMakeFiles/database_connect_pool.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/database_connect_pool.dir/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/database_connect_pool/main.cpp -o CMakeFiles/database_connect_pool.dir/main.cpp.s

CMakeFiles/database_connect_pool.dir/src/connectionPool.cpp.o: CMakeFiles/database_connect_pool.dir/flags.make
CMakeFiles/database_connect_pool.dir/src/connectionPool.cpp.o: /root/database_connect_pool/src/connectionPool.cpp
CMakeFiles/database_connect_pool.dir/src/connectionPool.cpp.o: CMakeFiles/database_connect_pool.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/root/database_connect_pool/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/database_connect_pool.dir/src/connectionPool.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/database_connect_pool.dir/src/connectionPool.cpp.o -MF CMakeFiles/database_connect_pool.dir/src/connectionPool.cpp.o.d -o CMakeFiles/database_connect_pool.dir/src/connectionPool.cpp.o -c /root/database_connect_pool/src/connectionPool.cpp

CMakeFiles/database_connect_pool.dir/src/connectionPool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/database_connect_pool.dir/src/connectionPool.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/database_connect_pool/src/connectionPool.cpp > CMakeFiles/database_connect_pool.dir/src/connectionPool.cpp.i

CMakeFiles/database_connect_pool.dir/src/connectionPool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/database_connect_pool.dir/src/connectionPool.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/database_connect_pool/src/connectionPool.cpp -o CMakeFiles/database_connect_pool.dir/src/connectionPool.cpp.s

CMakeFiles/database_connect_pool.dir/src/databaseConnect.cpp.o: CMakeFiles/database_connect_pool.dir/flags.make
CMakeFiles/database_connect_pool.dir/src/databaseConnect.cpp.o: /root/database_connect_pool/src/databaseConnect.cpp
CMakeFiles/database_connect_pool.dir/src/databaseConnect.cpp.o: CMakeFiles/database_connect_pool.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/root/database_connect_pool/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/database_connect_pool.dir/src/databaseConnect.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/database_connect_pool.dir/src/databaseConnect.cpp.o -MF CMakeFiles/database_connect_pool.dir/src/databaseConnect.cpp.o.d -o CMakeFiles/database_connect_pool.dir/src/databaseConnect.cpp.o -c /root/database_connect_pool/src/databaseConnect.cpp

CMakeFiles/database_connect_pool.dir/src/databaseConnect.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/database_connect_pool.dir/src/databaseConnect.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/database_connect_pool/src/databaseConnect.cpp > CMakeFiles/database_connect_pool.dir/src/databaseConnect.cpp.i

CMakeFiles/database_connect_pool.dir/src/databaseConnect.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/database_connect_pool.dir/src/databaseConnect.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/database_connect_pool/src/databaseConnect.cpp -o CMakeFiles/database_connect_pool.dir/src/databaseConnect.cpp.s

# Object files for target database_connect_pool
database_connect_pool_OBJECTS = \
"CMakeFiles/database_connect_pool.dir/main.cpp.o" \
"CMakeFiles/database_connect_pool.dir/src/connectionPool.cpp.o" \
"CMakeFiles/database_connect_pool.dir/src/databaseConnect.cpp.o"

# External object files for target database_connect_pool
database_connect_pool_EXTERNAL_OBJECTS =

/root/database_connect_pool/bin/database_connect_pool: CMakeFiles/database_connect_pool.dir/main.cpp.o
/root/database_connect_pool/bin/database_connect_pool: CMakeFiles/database_connect_pool.dir/src/connectionPool.cpp.o
/root/database_connect_pool/bin/database_connect_pool: CMakeFiles/database_connect_pool.dir/src/databaseConnect.cpp.o
/root/database_connect_pool/bin/database_connect_pool: CMakeFiles/database_connect_pool.dir/build.make
/root/database_connect_pool/bin/database_connect_pool: CMakeFiles/database_connect_pool.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/root/database_connect_pool/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable /root/database_connect_pool/bin/database_connect_pool"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/database_connect_pool.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/database_connect_pool.dir/build: /root/database_connect_pool/bin/database_connect_pool
.PHONY : CMakeFiles/database_connect_pool.dir/build

CMakeFiles/database_connect_pool.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/database_connect_pool.dir/cmake_clean.cmake
.PHONY : CMakeFiles/database_connect_pool.dir/clean

CMakeFiles/database_connect_pool.dir/depend:
	cd /root/database_connect_pool/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/database_connect_pool /root/database_connect_pool /root/database_connect_pool/build /root/database_connect_pool/build /root/database_connect_pool/build/CMakeFiles/database_connect_pool.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/database_connect_pool.dir/depend

