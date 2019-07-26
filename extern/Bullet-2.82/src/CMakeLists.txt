project(Bullet)

file(GLOB_RECURSE BulletCollision ${CMAKE_CURRENT_SOURCE_DIR}/BulletCollision/*.cpp)
source_group("BulletCollision" FILES ${BulletCollision})

file(GLOB_RECURSE BulletDynamics ${CMAKE_CURRENT_SOURCE_DIR}/BulletDynamics/*.cpp)
source_group("BulletDynamics" FILES ${BulletDynamics})

file(GLOB_RECURSE LinearMath ${CMAKE_CURRENT_SOURCE_DIR}/LinearMath/*.cpp)
source_group("LinearMath" FILES ${LinearMath})

add_library(${PROJECT_NAME} STATIC ${BulletCollision} ${BulletDynamics} ${LinearMath})

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/"
	"${CMAKE_CURRENT_SOURCE_DIR}/../"
)

if (WIN32)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "extern") 