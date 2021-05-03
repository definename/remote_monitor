project(remote_server)

set(SOURCE_FILES
	src/remote_server.cpp
	)
set(HEADER_FILES
	src/remote_server.h
	src/project_header.h
	)

source_group("Source files" FILES ${SOURCE_FILES})
source_group("Header files" FILES ${HEADER_FILES})

add_executable(${PROJECT_NAME}
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
		${ZLIB_INCLUDE_DIRS}
		${Protobuf_INCLUDE_DIRS}
		${Protogen_INCLUDE_DIRS}
		${NETLIB_INCLUDE_DIRS}
		)
target_link_libraries(${PROJECT_NAME}
	PRIVATE
		${Boost_LIBRARIES}
		${POCO_LIBRARIES}
		${ZLIB_LIBRARIES}
		${Protobuf_LIBRARIES}
		${Protogen_LIBRARIES}
		${wxWidgets_LIBRARIES}
		${NETLIB_LIBRARIES}
		)
add_dependencies(${PROJECT_NAME}
	pocoproject
	netlib
	protogen
	)