cmake_minimum_required(VERSION 3.30)

project(client)

set(sources
	${STUBS}
	${STUBS_DIR}/CallbackAgentS.cpp
	CallbackAgentI.cpp
	CMQ.cpp
	Callback.cpp
	main_client.cpp)

add_executable(${PROJECT_NAME} ${sources})
add_dependencies(${PROJECT_NAME} ${CMAKE_PROJECT_NAME}_stubs)
target_compile_options(${PROJECT_NAME} PUBLIC)
target_link_libraries(${PROJECT_NAME}
	${mylibs} spdlog::spdlog_header_only)
