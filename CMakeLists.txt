cmake_minimum_required(VERSION 3.19)

if(WIN32)
	set(BUILD_WITH_MSVC_STATIC_RUNTIME_LIBRARY OFF CACHE BOOL "Select the MSVC static runtime")
	if(${BUILD_WITH_MSVC_STATIC_RUNTIME_LIBRARY})
		set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	else()
		set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
	endif()
else()
	message(FATAL_ERROR "Unsupported target system")
endif()

project(remote_monitor)

# Where to put all the ARCHIVE targets when built.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# Where to put all the RUNTIME target files when built.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(remote_server)
add_subdirectory(remote_client)