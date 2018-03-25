project(Application)

message(STATUS "Configuring Application")

set(SOURCE_FILES
		Module.h
		Module.cpp
		)

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/../"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern/include"
)

link_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern_vs/Win32/Debug"
)

if (WIN32)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

	add_definitions(-DWIN32)

	add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
else (WIN32)
	add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
endif()


target_link_libraries(${PROJECT_NAME} common engine)


add_definitions(-D_USRDLL)
add_definitions(-DAPP_EXPORTS)
