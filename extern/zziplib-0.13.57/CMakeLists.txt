project(zziplib)


file(GLOB_RECURSE zzip ${CMAKE_CURRENT_SOURCE_DIR}/zzip/*.c)
source_group("zzip" FILES ${zzip})

add_library(${PROJECT_NAME} STATIC 
    ${zzip}
    )

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/"
	"${CMAKE_CURRENT_SOURCE_DIR}/../zlib-1.2.3"
)

if (WIN32)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

	add_definitions(-DWIN32)
	add_definitions(-D_LIB)
endif()
