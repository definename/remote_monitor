cmake_minimum_required(VERSION 3.15)

project(protogen)

set(BUILD_WITH_MSVC_STATIC_RUNTIME_LIBRARY OFF CACHE BOOL "Select the MSVC static runtime")
message(STATUS "BUILD_WITH_MSVC_STATIC_RUNTIME_LIBRARY was set to: ${BUILD_WITH_MSVC_STATIC_RUNTIME_LIBRARY}")
if(${BUILD_WITH_MSVC_STATIC_RUNTIME_LIBRARY})
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	message(STATUS "static runtime is being used")
else()
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
	message(STATUS "dynamic runtime is being used")
endif()

# list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
# include(CRTLinkage)

if (EXISTS ${Protofiles_ROOT})
	file(GLOB PROTOFILES "${Protofiles_ROOT}/*.proto")
	message(STATUS "Protofiles to be processed: ${PROTOFILES}")
else()
	message(FATAL_ERROR "Protofile root path is invalid")
endif()

if (EXISTS ${Protobuf_ROOT})
	message(STATUS "Protobuf root directory path: ${Protobuf_ROOT}")
else()
	message(FATAL_ERROR "Protobuf root path is invalid")
endif()

set(Protobuf_INCLUDE_DIR ${Protobuf_ROOT}/include)
set(Protobuf_PROTOC_EXECUTABLE ${Protobuf_ROOT}/bin/protoc.exe)
set(Protobuf_LIBRARY
	debug ${Protobuf_ROOT}/lib/libprotobufd.lib
	optimized ${Protobuf_ROOT}/lib/libprotobuf.lib
	)
set(Protobuf_PROTOC_LIBRARY
	debug ${Protobuf_ROOT}/lib/libprotocd.lib
	optimized ${Protobuf_ROOT}/lib/libprotoc.lib
	)
set(Protobuf_LITE_LIBRARY
	debug ${Protobuf_ROOT}/lib/libprotobuf-lited.lib
	optimized ${Protobuf_ROOT}/lib/libprotobuf-lite.lib
	)
set(Protobuf_DEBUG ON)
set(Protobuf_USE_STATIC_LIBS ON)

find_package(Protobuf REQUIRED)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTOFILES})

add_library(${PROJECT_NAME} STATIC
	${PROTO_SRCS}
	${PROTO_HDRS}
	)

target_include_directories(${PROJECT_NAME} PUBLIC
	${Protobuf_INCLUDE_DIRS}
	)

install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES ${PROTO_HDRS} ${PROTO_SRCS} DESTINATION ${CMAKE_INSTALL_PREFIX})
