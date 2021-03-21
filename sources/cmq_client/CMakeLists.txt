cmake_minimum_required(VERSION 3.12)

project(cmq_client)

set(sources
	${STUBS}
	${IDL_DIR}/CallbackAgentS.cpp
	CallbackAgentI.cpp
	main_client.cpp)

add_executable(${PROJECT_NAME} ${sources})

target_link_libraries(${PROJECT_NAME} ${MYLIBS} -lreadline)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "${PROJECT_SOURCE_DIR}/../bin")
