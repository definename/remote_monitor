project(netlib)

set(SOURCE_FILES
	src/netlib_server.cpp
	src/netlib_core.cpp
	)
set(HEADER_FILES
	src/netlib_server.h
	src/netlib_core.h
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