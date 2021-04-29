project(remote_client)
set(SRC src/main.cpp)
add_executable(${PROJECT_NAME} ${SRC})
target_precompile_headers(${PROJECT_NAME}
	PRIVATE
		src/project_header.h
		)