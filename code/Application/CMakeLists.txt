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
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern/zziplib-0.13.57/msvc8"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern/FreeImage-3.13.1/Source/FreeImageLib/Debug"
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} common engine)

add_definitions(-DWIN32)
add_definitions(-D_USRDLL)
add_definitions(-DAPP_EXPORTS)
