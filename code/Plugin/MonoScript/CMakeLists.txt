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
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern_vs/Win32/Debug"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/vulkan/lib"
)

set(EXTERN_LINK_LIB
${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/mono/lib/mono-2.0.lib
)

if (WIN32)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

	add_definitions(-DWIN32)
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} Common Engine ${EXTERN_LINK_LIB})


add_definitions(-DUSRDLL)
add_definitions(-DMONOSCRIPT_EXPORTS)
