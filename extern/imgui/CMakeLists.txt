project(imgui)


file(GLOB_RECURSE imgui 
	${CMAKE_CURRENT_SOURCE_DIR}/imgui.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/imgui_draw.cpp
	)
source_group("imgui" FILES ${imgui})

add_library(${PROJECT_NAME} STATIC 
    ${imgui}
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