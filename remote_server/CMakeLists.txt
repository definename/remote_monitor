project(remote_server)
set(SRC src/main.cpp)
add_executable(${PROJECT_NAME} ${SRC})
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
		${wxWidgets_LIBRARIES}
		)