project(remote_client_gui)

configure_file(src/res/app.rc.cmake app.rc @ONLY)

set(SOURCE_FILES
	src/client_frame.cpp
	src/client_app.cpp
	src/client_panel.cpp
	)
set(HEADER_FILES
	src/project_header.h
	src/client_frame.h
	src/client_app.h
	src/client_panel.h
	)
set(RESOURCE_FILES
	src/res/frame.ico
	${CMAKE_CURRENT_BINARY_DIR}/app.rc
	)

set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/app.rc PROPERTIES GENERATED 1)

source_group("Source files" FILES ${SOURCE_FILES})
source_group("Header files" FILES ${HEADER_FILES})
source_group("Resource Files" FILES ${RESOURCE_FILES})

add_executable(${PROJECT_NAME} WIN32
	${SOURCE_FILES}
	${HEADER_FILES}
	${RESOURCE_FILES}
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
		# ${Boost_LIBRARIES}
		${POCO_LIBRARIES}
		${ZLIB_LIBRARIES}
		${Protobuf_LIBRARIES}
		${Protogen_LIBRARIES}
		${wxWidgets_LIBRARIES}
		${NETLIB_LIBRARIES}
		)
add_dependencies(${PROJECT_NAME}
	pocoproject
	protogen
	netlib
	)
add_compile_definitions(WIN32_LEAN_AND_MEAN)