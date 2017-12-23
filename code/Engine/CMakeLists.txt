project(engine)

message(STATUS "Configuring engine")

set(SOURCE_FILES
		Module.h
		Module.cpp 
		Engine.def)

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/../"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern/zziplib-0.13.57"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern/FreeImage-3.13.1/Source"
)

link_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern/zziplib-0.13.57/msvc8"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern/FreeImage-3.13.1/Source/FreeImageLib/Debug"
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} common zziplibd.lib FreeImaged.lib)

