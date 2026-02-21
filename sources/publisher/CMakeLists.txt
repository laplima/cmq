cmake_minimum_required(VERSION 3.30)

project(publisher)   # name of the executable

set(sources
	${STUBS}
	CMQ.cpp
	main_publisher.cpp)

add_executable(${PROJECT_NAME} ${sources})
add_dependencies(${PROJECT_NAME} ${CMAKE_PROJECT_NAME}_stubs)
target_compile_options(${PROJECT_NAME} PUBLIC)
target_link_libraries(${PROJECT_NAME}
	${mylibs} spdlog::spdlog_header_only)
