cmake_minimum_required(VERSION 3.20)

project(client)

set(sources
	${STUBS}
	${STUBS_DIR}/CallbackAgentS.cpp
	CallbackAgentI.cpp
	CMQ.cpp
	Callback.cpp
	main_client.cpp)

include("${MYMODULES_DIR}/taoidl.cmake")

add_executable(${PROJECT_NAME} ${sources})
target_compile_options(${PROJECT_NAME} PUBLIC)
target_link_libraries(${PROJECT_NAME} ${mylibs} fmt::fmt)
