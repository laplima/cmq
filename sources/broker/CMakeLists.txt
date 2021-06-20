cmake_minimum_required(VERSION 3.17)

project(broker)

set(sources
	${STUBS}
	${SKELETONS}
	CMQI.cpp
	Exchange.cpp
	Queue.cpp
	main_server.cpp)

add_executable(${PROJECT_NAME} ${sources})

target_link_libraries(${PROJECT_NAME} ${MYLIBS})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "${PROJECT_SOURCE_DIR}/../bin")
