project(BulletPhysics)

message(STATUS "Configuring BulletPhysics")

set(SOURCE_FILES
		Module.h
		Module.cpp
		Export.def
		)

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/../"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/Bullet-2.82/src"
)


if (WIN32)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

	add_definitions(-DWIN32)

	add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
else (WIN32)
	add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
endif()



target_link_libraries(${PROJECT_NAME} Common Engine Bullet)


add_definitions(-DUSRDLL)
add_definitions(-DBULLETPHYSICS_EXPORTS)

