cmake_minimum_required(VERSION 3.30)

project(consumer)

set(sources
	${STUBS}
	${STUBS_DIR}/CallbackAgentS.cpp
	CMQ.cpp
	Callback.cpp
	CallbackAgentI.cpp
	main_consumer.cpp)

add_executable(${PROJECT_NAME} ${sources})
add_dependencies(${PROJECT_NAME} ${CMAKE_PROJECT_NAME}_stubs)
target_compile_options(${PROJECT_NAME} PUBLIC)
target_link_libraries(${PROJECT_NAME}
	${mylibs} spdlog::spdlog_header_only)
