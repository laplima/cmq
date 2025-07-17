cmake_minimum_required(VERSION 3.20)

project(publisher)   # name of the executable

set(sources
	${STUBS}
	CMQ.cpp
	main_publisher.cpp)

include("${MYMODULES_DIR}/taoidl.cmake")

add_executable(${PROJECT_NAME} ${sources})
target_compile_options(${PROJECT_NAME} PUBLIC)
target_link_libraries(${PROJECT_NAME} ${mylibs})
