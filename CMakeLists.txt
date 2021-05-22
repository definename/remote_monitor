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
include(ExternalProject)

# Where to put all the ARCHIVE targets when built.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# Where to put all the RUNTIME target files when built.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Opensource path
set(OPENSOURCE_PATH ${CMAKE_SOURCE_DIR}/opensource)
# Patch directory path
set(PATCH_PATH ${OPENSOURCE_PATH}/patches)
# Opensource install path
set(OPENSOURCE_INSTALL_PATH ${CMAKE_BINARY_DIR}/opensource)
# Opensource root dirs
set(POCO_ROOT ${OPENSOURCE_INSTALL_PATH}/Poco)
set(ZLIB_ROOT ${OPENSOURCE_INSTALL_PATH}/zlib)
set(Protobuf_ROOT ${OPENSOURCE_INSTALL_PATH}/protobuf)

###########################################################
# Boost ###################################################

cmake_policy(SET CMP0074 NEW)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ${BUILD_WITH_MSVC_STATIC_RUNTIME_LIBRARY})
find_package(Boost REQUIRED COMPONENTS serialization filesystem date_time regex)

###########################################################
# wxWidgets ###############################################

set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets COMPONENTS core base adv net webview REQUIRED)
include(${wxWidgets_USE_FILE})

###########################################################
# Poco ####################################################

ExternalProject_Add(
	pocoproject
	URL "${OPENSOURCE_PATH}/poco-1.7.0-all.zip"
	URL_MD5 C5F0EEC1EEDF84323E0991E911465D8D
	CMAKE_ARGS
		-DENABLE_XML:BOOL=ON
		-DENABLE_JSON:BOOL=ON
		-DENABLE_MONGODB:BOOL=OFF			
		-DENABLE_PDF:BOOL=OFF
		-DENABLE_UTIL:BOOL=ON
		-DENABLE_NET:BOOL=ON
		-DENABLE_NETSSL:BOOL=ON
		-DENABLE_NETSSL_WIN:BOOL=OFF
		-DENABLE_CRYPTO:BOOL=ON
		-DENABLE_DATA:BOOL=OFF
		-DENABLE_SEVENZIP:BOOL=OFF
		-DENABLE_ZIP:BOOL=OFF
		-DENABLE_CPPPARSER:BOOL=OFF
		-DENABLE_POCODOC:BOOL=OFF
		-DENABLE_PAGECOMPILER:BOOL=OFF
		-DENABLE_PAGECOMPILER_FILE2PAGE:BOOL=OFF
		-DPOCO_STATIC:BOOL=ON
		-DPOCO_UNBUNDLED:BOOL=OFF
		-DPOCO_MT:BOOL=ON
		-DENABLE_MSVC_MP:BOOL=ON
		-DCMAKE_INSTALL_PREFIX:PATH=${POCO_ROOT}
	PATCH_COMMAND
		${CMAKE_COMMAND} -P ${PATCH_PATH}/poco-1.7.0-all.patches/patch.cmake ${CMAKE_SOURCE_DIR}
	)
set(POCO_INCLUDE_DIR ${POCO_ROOT}/include)
set(POCO_LIBRARIES
	# ${POCO_ROOT}/lib/PocoCrypto.lib
	${POCO_ROOT}/lib/PocoFoundation.lib
	${POCO_ROOT}/lib/PocoJSON.lib
	${POCO_ROOT}/lib/PocoNet.lib
	${POCO_ROOT}/lib/PocoUtil.lib
	# ${POCO_ROOT}/lib/PocoNetSSL.lib
	${POCO_ROOT}/lib/PocoXML.lib
	Iphlpapi.lib # To prevent linkage error
	)

###########################################################
# zlib ####################################################

ExternalProject_Add(
	zlib
	URL "${OPENSOURCE_PATH}/zlib-1.2.8.zip"
	URL_MD5 D34BAA0DA16E33A5EF095E7EEF7D2472
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX:PATH=${ZLIB_ROOT}
		-DBUILD_SHARED_LIBS:BOOL=OFF
		-DBUILD_WITH_MSVC_STATIC_RUNTIME_LIBRARY:BOOL=${BUILD_WITH_MSVC_STATIC_RUNTIME_LIBRARY}
	PATCH_COMMAND 
		${CMAKE_COMMAND} -P ${PATCH_PATH}/zlib-1.2.8.patches/patch.cmake ${CMAKE_SOURCE_DIR}
	)

set(ZLIB_INCLUDE_DIRS ${ZLIB_ROOT}/include)
set(ZLIB_LIBRARIES
	debug ${ZLIB_ROOT}/lib/zlibstaticd.lib
	optimized ${ZLIB_ROOT}/lib/zlibstatic.lib
	)

############################################################
# PROTOBUF-CPP #############################################

ExternalProject_Add(
	protobuf
	DEPENDS
		zlib
	URL "${OPENSOURCE_PATH}/protobuf-cpp-3.7.1.zip"
	URL_MD5 7d819ec1943f1fef3a85aaad32e122f4
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX:PATH=${Protobuf_ROOT}
		-DZLIB_ROOT:PATH=${ZLIB_ROOT}
		-Dprotobuf_BUILD_TESTS=OFF
		-DBUILD_SHARED_LIBS=OFF
		-Dprotobuf_VERBOSE=ON
	SOURCE_SUBDIR cmake
	PATCH_COMMAND
		${CMAKE_COMMAND} -P ${PATCH_PATH}/protobuf-cpp-3.7.1.patches/patch.cmake ${CMAKE_SOURCE_DIR}
	)
set(Protobuf_INCLUDE_DIRS ${Protobuf_ROOT}/include)
set(Protobuf_LIBRARIES
	debug ${Protobuf_ROOT}/lib/libprotobufd.lib
	optimized ${Protobuf_ROOT}/lib/libprotobuf.lib
	)

set(Protofiles_ROOT ${CMAKE_SOURCE_DIR}/protofiles)
set(Protogen_ROOT ${OPENSOURCE_INSTALL_PATH}/protogen)
set(Protogen_INCLUDE_DIRS ${Protogen_ROOT})
set(Protogen_LIBRARIES ${Protogen_ROOT}/protogen.lib)
ExternalProject_Add(
	protogen
	SOURCE_DIR ${OPENSOURCE_PATH}/protogen-1.0
	DEPENDS
		protobuf
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX:PATH=${Protogen_ROOT}
		-DBUILD_WITH_MSVC_STATIC_RUNTIME_LIBRARY:BOOL=${BUILD_WITH_MSVC_STATIC_RUNTIME_LIBRARY}
		-DProtobuf_ROOT=${Protobuf_ROOT}
		-DProtofiles_ROOT=${Protofiles_ROOT}
	)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
SET_PROPERTY(TARGET pocoproject PROPERTY FOLDER "opensource")
SET_PROPERTY(TARGET zlib PROPERTY FOLDER "opensource")
SET_PROPERTY(TARGET protobuf PROPERTY FOLDER "opensource")
SET_PROPERTY(TARGET protogen PROPERTY FOLDER "opensource")

add_subdirectory(netlib)
add_subdirectory(monitor_daemon)
add_subdirectory(monitor_client_gui)
add_subdirectory(monitor_client)