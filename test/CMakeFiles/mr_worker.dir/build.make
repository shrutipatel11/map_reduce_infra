# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /workspace

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /workspace

# Include any dependencies generated for this target.
include test/CMakeFiles/mr_worker.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/mr_worker.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/mr_worker.dir/flags.make

test/CMakeFiles/mr_worker.dir/user_tasks.cc.o: test/CMakeFiles/mr_worker.dir/flags.make
test/CMakeFiles/mr_worker.dir/user_tasks.cc.o: test/user_tasks.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/mr_worker.dir/user_tasks.cc.o"
	cd /workspace/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mr_worker.dir/user_tasks.cc.o -c /workspace/test/user_tasks.cc

test/CMakeFiles/mr_worker.dir/user_tasks.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mr_worker.dir/user_tasks.cc.i"
	cd /workspace/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspace/test/user_tasks.cc > CMakeFiles/mr_worker.dir/user_tasks.cc.i

test/CMakeFiles/mr_worker.dir/user_tasks.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mr_worker.dir/user_tasks.cc.s"
	cd /workspace/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspace/test/user_tasks.cc -o CMakeFiles/mr_worker.dir/user_tasks.cc.s

# Object files for target mr_worker
mr_worker_OBJECTS = \
"CMakeFiles/mr_worker.dir/user_tasks.cc.o"

# External object files for target mr_worker
mr_worker_EXTERNAL_OBJECTS =

bin/mr_worker: test/CMakeFiles/mr_worker.dir/user_tasks.cc.o
bin/mr_worker: test/CMakeFiles/mr_worker.dir/build.make
bin/mr_worker: src/libmr_workerlib.a
bin/mr_worker: src/libp4protolib.a
bin/mr_worker: /usr/local/lib/libgrpc++.a
bin/mr_worker: /usr/local/lib/libprotobuf.a
bin/mr_worker: /usr/local/lib/libgrpc.a
bin/mr_worker: /usr/local/lib/libssl.a
bin/mr_worker: /usr/local/lib/libcrypto.a
bin/mr_worker: /usr/local/lib/libz.a
bin/mr_worker: /usr/local/lib/libcares.a
bin/mr_worker: /usr/local/lib/libre2.a
bin/mr_worker: /usr/local/lib/libabsl_statusor.a
bin/mr_worker: /usr/local/lib/libabsl_hash.a
bin/mr_worker: /usr/local/lib/libabsl_bad_variant_access.a
bin/mr_worker: /usr/local/lib/libabsl_city.a
bin/mr_worker: /usr/local/lib/libabsl_raw_hash_set.a
bin/mr_worker: /usr/local/lib/libabsl_hashtablez_sampler.a
bin/mr_worker: /usr/local/lib/libabsl_exponential_biased.a
bin/mr_worker: /usr/local/lib/libgpr.a
bin/mr_worker: /usr/local/lib/libabsl_status.a
bin/mr_worker: /usr/local/lib/libabsl_cord.a
bin/mr_worker: /usr/local/lib/libabsl_bad_optional_access.a
bin/mr_worker: /usr/local/lib/libabsl_synchronization.a
bin/mr_worker: /usr/local/lib/libabsl_stacktrace.a
bin/mr_worker: /usr/local/lib/libabsl_symbolize.a
bin/mr_worker: /usr/local/lib/libabsl_debugging_internal.a
bin/mr_worker: /usr/local/lib/libabsl_demangle_internal.a
bin/mr_worker: /usr/local/lib/libabsl_graphcycles_internal.a
bin/mr_worker: /usr/local/lib/libabsl_time.a
bin/mr_worker: /usr/local/lib/libabsl_civil_time.a
bin/mr_worker: /usr/local/lib/libabsl_time_zone.a
bin/mr_worker: /usr/local/lib/libabsl_malloc_internal.a
bin/mr_worker: /usr/local/lib/libabsl_str_format_internal.a
bin/mr_worker: /usr/local/lib/libabsl_strings.a
bin/mr_worker: /usr/local/lib/libabsl_strings_internal.a
bin/mr_worker: /usr/local/lib/libabsl_int128.a
bin/mr_worker: /usr/local/lib/libabsl_throw_delegate.a
bin/mr_worker: /usr/local/lib/libabsl_base.a
bin/mr_worker: /usr/local/lib/libabsl_raw_logging_internal.a
bin/mr_worker: /usr/local/lib/libabsl_log_severity.a
bin/mr_worker: /usr/local/lib/libabsl_spinlock_wait.a
bin/mr_worker: /usr/local/lib/libaddress_sorting.a
bin/mr_worker: /usr/local/lib/libupb.a
bin/mr_worker: test/CMakeFiles/mr_worker.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/workspace/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/mr_worker"
	cd /workspace/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mr_worker.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/mr_worker.dir/build: bin/mr_worker

.PHONY : test/CMakeFiles/mr_worker.dir/build

test/CMakeFiles/mr_worker.dir/clean:
	cd /workspace/test && $(CMAKE_COMMAND) -P CMakeFiles/mr_worker.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/mr_worker.dir/clean

test/CMakeFiles/mr_worker.dir/depend:
	cd /workspace && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /workspace /workspace/test /workspace /workspace/test /workspace/test/CMakeFiles/mr_worker.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/mr_worker.dir/depend

