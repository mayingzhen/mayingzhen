project(common)

message(STATUS "Configuring common")

set(SOURCE_FILES
		Module.h
		Module.cpp
		)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
