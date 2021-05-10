project(netlib)

set(SOURCE_FILES
	src/netlib_client.cpp
	src/netlib_server.cpp
	src/netlib_session.cpp
	src/netlib_core.cpp
	src/netlib_util.cpp
	src/netlib_message.cpp
	src/netlib_mgr.cpp
	src/netlib_sender.cpp
	src/netlib_signal.cpp
	)
set(HEADER_FILES
	src/netlib_client.h
	src/netlib_server.h
	src/netlib_session.h
	src/netlib_core.h
	src/netlib_util.h
	src/netlib_message.h
	src/netlib_mgr.h
	src/netlib_sender.h
	src/netlib_signal.h
	
	src/netlib_log.h
	src/project_header.h
	)

source_group("Source files" FILES ${SOURCE_FILES})
source_group("Header files" FILES ${HEADER_FILES})

add_library(${PROJECT_NAME} STATIC
	${SOURCE_FILES}
	${HEADER_FILES}
	)

target_precompile_headers(${PROJECT_NAME}
	PRIVATE
		src/project_header.h
		)
target_include_directories(${PROJECT_NAME}
	PRIVATE
		${Boost_INCLUDE_DIRS}
		${POCO_INCLUDE_DIR}
		${Protobuf_INCLUDE_DIRS}
		${Protogen_INCLUDE_DIRS}
		)
target_link_libraries(${PROJECT_NAME}
	PRIVATE
		${Boost_LIBRARIES}
		${POCO_LIBRARIES}
		)
add_dependencies(${PROJECT_NAME}
	pocoproject
	)
set(NETLIB_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/netlib/src PARENT_SCOPE)
set(NETLIB_LIBRARIES ${PROJECT_NAME} PARENT_SCOPE)