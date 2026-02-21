cmake_minimum_required(VERSION 3.30)

project(broker)

set(sources
	${SKELETONS}
	CMQI.cpp
	Exchange.cpp
	Queue.cpp
	main_server.cpp)

add_executable(${PROJECT_NAME})
add_dependencies(${PROJECT_NAME} ${CMAKE_PROJECT_NAME}_stubs)
target_sources(${PROJECT_NAME} PRIVATE ${sources})
target_compile_options(${PROJECT_NAME} PUBLIC)
target_link_libraries(${PROJECT_NAME}
	${mylibs} spdlog::spdlog_header_only)
