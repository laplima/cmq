cmake_minimum_required(VERSION 3.17)

project(publisher)

set(sources
	${STUBS}
	CMQ.cpp
	main_publisher.cpp)

add_executable(${PROJECT_NAME} ${sources})

target_link_libraries(${PROJECT_NAME} ${MYLIBS})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "${PROJECT_SOURCE_DIR}/../bin")
