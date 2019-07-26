project(Engine)

message(STATUS "Configuring Engine")

set(SOURCE_FILES
		Module.h
		Module.cpp 
		Export.def)

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/../"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern/zziplib-0.13.57"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern/FreeImage-3.13.1/Source"
)




if (WIN32)
	add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

	target_link_libraries(${PROJECT_NAME} Common FreeImage zziplib zlib)
	
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj")
else (WIN32)
	add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

	target_link_libraries(${PROJECT_NAME} Common FreeImage zziplib zlib log android)
endif()




