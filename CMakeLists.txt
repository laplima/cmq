cmake_minimum_required(VERSION 3.12)

project(cmq)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Release)

set(BIN_DIR bin)

set(IDL CMQ CallbackAgent)
set(IDL_DIR idl)

# MYLIBS

set(MYLIBS)

set(libs
	ACE TAO
	TAO_PortableServer
	TAO_AnyTypeCode
	TAO_CosNaming
	TAO_IORTable
	colibry_static
	readline)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(libs ${libs} pthread)
endif()

foreach(lib ${libs})
	set(the_lib ${lib}-NOTFOUND)
	find_library(the_lib ${lib} PATHS "$ENV{ACE_ROOT}/lib" "$ENV{COLIBRY}/lib")
	set(MYLIBS ${MYLIBS} ${the_lib})
endforeach(lib)

# IDL

set(IDL_DIR "${PROJECT_SOURCE_DIR}/${IDL_DIR}")

set(IDL_FILES)
set(IDL_IMPL)
set(IDL_OUTPUT)
set(SKELETONS)
set(STUBS)
foreach(idl ${IDL})
	set(SKELETONS ${SKELETONS} ${IDL_DIR}/${idl}S.cpp)
	set(STUBS ${STUBS} ${IDL_DIR}/${idl}C.cpp)
	set(IDL_OUTPUT ${IDL_OUTPUT}
		${IDL_DIR}/${idl}C.h ${IDL_DIR}/${idl}C.inl ${IDL_DIR}/${idl}S.h)
	set(IDL_FILES ${IDL_FILES} ${IDL_DIR}/${idl}.idl)
	set(IDL_IMPL ${IDL_IMPL} ${idl}I.cpp)
endforeach(idl)
set(IDL_OUTPUT ${STUBS} ${SKELETONS} ${IDL_OUTPUT})

add_custom_command(
	OUTPUT ${IDL_OUTPUT}
	DEPENDS ${IDL_FILES}
	COMMAND tao_idl ${IDL_FILES}
	WORKING_DIRECTORY ${IDL_DIR}
	COMMENT "Generating stubs + skeletons"
	VERBATIM)

add_custom_command(
	OUTPUT ${IDL_OUTPUT}
	DEPENDS ${IDL_FILES}
	COMMAND tao_idl ${IDL_FILES}
	WORKING_DIRECTORY ${IDL_DIR}
	COMMENT "Generating stubs + skeletons"
	VERBATIM)

# INCLUDES

include_directories(
    "${IDL_DIR}"
    "$ENV{COLIBRY}/include"
    "$ENV{ACE_ROOT}"
    "$ENV{TAO_ROOT}"
    "$ENV{TAO_ROOT}/orbsvcs")

add_compile_options(-Wno-deprecated-declarations -fdelayed-template-parsing)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/${BIN_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

# Subprojects

add_subdirectory(sources/cmq_server)
add_subdirectory(sources/cmq_producer)
add_subdirectory(sources/cmq_consumer)
add_subdirectory(sources/cmq_client)
