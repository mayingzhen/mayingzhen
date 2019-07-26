project(zlib)


file(GLOB_RECURSE zlib ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
source_group("zlib" FILES ${zlib})

add_library(${PROJECT_NAME} STATIC 
    ${zlib}
    )

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/"
)

if (WIN32)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

	add_definitions(-DWIN32)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "extern") 