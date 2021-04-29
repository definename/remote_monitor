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
# Poco root directory
set(POCO_ROOT ${OPENSOURCE_INSTALL_PATH}/Poco)

###########################################################
# Boost ###################################################

cmake_policy(SET CMP0074 NEW)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ${BUILD_WITH_MSVC_STATIC_RUNTIME_LIBRARY})
find_package(Boost REQUIRED COMPONENTS serialization filesystem)

###########################################################
# wxWidgets ###############################################

set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets COMPONENTS core base adv net webview REQUIRED)
include(${wxWidgets_USE_FILE})

###########################################################
# Poco ####################################################

ExternalProject_Add(
	Poco
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
	)

add_subdirectory(remote_server)
add_subdirectory(remote_client)