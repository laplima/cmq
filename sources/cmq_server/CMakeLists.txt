cmake_minimum_required(VERSION 3.12)

project(cmq_server)

set(sources
	${STUBS}
	${SKELETONS}
	CMQI.cpp
	main_server.cpp)

add_executable(${PROJECT_NAME} ${sources})

target_link_libraries(${PROJECT_NAME} ${MYLIBS})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "${PROJECT_SOURCE_DIR}/../bin")
