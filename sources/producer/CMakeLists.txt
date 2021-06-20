cmake_minimum_required(VERSION 3.17)

project(producer)

set(sources
	${STUBS}
	${IDL_DIR}/CallbackAgentS.cpp
	CallbackAgentI.cpp
	main_producer.cpp)

add_executable(${PROJECT_NAME} ${sources})

target_link_libraries(${PROJECT_NAME} ${MYLIBS})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "${PROJECT_SOURCE_DIR}/../bin")
