cmake_minimum_required(VERSION 3.10)

project(purray
	VERSION 1.0.0
	LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(purray STATIC
	src/purray.c
)

target_include_directories(purray
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(purray PRIVATE
	$<$<C_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic>
	$<$<C_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic>
	$<$<C_COMPILER_ID:MSVC>:/W4>
)

option(PURRAY_BUILD_EXAMPLES "Build examples" OFF)

if(PURRAY_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()

install(TARGETS purray
	EXPORT purray-targets
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin
)

install(FILES
	include/purray.h
	DESTINATION include
)

install(EXPORT purray-targets
	FILE purray-config.cmake
	NAMESPACE purray::
	DESTINATION lib/cmake/purray
)
