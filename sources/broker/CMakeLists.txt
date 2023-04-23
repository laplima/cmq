cmake_minimum_required(VERSION 3.20)

project(broker)

set(sources
	${SKELETONS}
	CMQI.cpp
	Exchange.cpp
	Queue.cpp
	main_server.cpp)

include("${MYMODULES_DIR}/taoidl.cmake")

add_executable(${PROJECT_NAME} ${sources})
target_compile_options(${PROJECT_NAME} PUBLIC ${FMT_CFLAGS_OTHER})
target_link_libraries(${PROJECT_NAME}
	${mylibs} ${FMT_LIBRARIES} ${FMT_LDFLAGS})
