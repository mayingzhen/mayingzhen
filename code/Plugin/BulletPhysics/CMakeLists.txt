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

link_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/zziplib-0.13.57/msvc8"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/FreeImage-3.13.1/Source/FreeImageLib/Debug"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/Bullet-2.82/build/vs2008/Lib"
)

set(EXTERN_LINK_LIB
BulletCollision_vs2008_debug.lib
BulletDynamics_vs2008_debug.lib
LinearMath_vs2008_debug.lib
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} common engine ${EXTERN_LINK_LIB})

add_definitions(-DWIN32)
add_definitions(-DUSRDLL)
add_definitions(-DBULLETPHYSICS_EXPORTS)

