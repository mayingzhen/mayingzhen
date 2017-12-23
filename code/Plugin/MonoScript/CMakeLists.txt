project(MonoScript)

message(STATUS "Configuring MonoScript")

set(SOURCE_FILES
		Module.h
		Module.cpp
		Export.def
		)

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/../"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/mono/Inlude"
)

link_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/zziplib-0.13.57/msvc8"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/FreeImage-3.13.1/Source/FreeImageLib/Debug"
)

set(EXTERN_LINK_LIB
${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/mono/lib/mono-2.0.lib
)

#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} common engine ${EXTERN_LINK_LIB})

add_definitions(-DWIN32)
add_definitions(-DUSRDLL)
add_definitions(-DMONOSCRIPT_EXPORTS)
