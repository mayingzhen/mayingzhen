project(Animation)

message(STATUS "Configuring Animation")

file(GLOB_RECURSE AnimationTree ${CMAKE_CURRENT_SOURCE_DIR}/AnimationTree/*.h*)
source_group("AnimationTree" FILES ${AnimationTree})

file(GLOB_RECURSE PoseModifier ${CMAKE_CURRENT_SOURCE_DIR}/PoseModifier/*.h*)
source_group("PoseModifier" FILES ${PoseModifier})

file(GLOB Animation ${CMAKE_CURRENT_SOURCE_DIR}/*.h*)

set(SOURCE_FILES
		${AnimationTree}
		${PoseModifier}
		${Animation}
		Module.h
		Module.cpp
		Export.def
		)

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/../"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/include"
)

link_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern_vs/Win32/Debug"
)

if (WIN32)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

	add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
else (WIN32)
	add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
endif()


target_link_libraries(${PROJECT_NAME} Common Engine)

