cmake_minimum_required(VERSION 2.8)

project(SampleBrowser)

message(STATUS "Configuring SampleBrowser")

file(GLOB_RECURSE Animation ${CMAKE_CURRENT_SOURCE_DIR}/Animation/*.h*)
source_group("Animation" FILES ${Animation})

file(GLOB_RECURSE Render ${CMAKE_CURRENT_SOURCE_DIR}/Render/*.h*)
source_group("Render" FILES ${Render})

file(GLOB_RECURSE Serialize ${CMAKE_CURRENT_SOURCE_DIR}/Serialize/*.h*)
source_group("Serialize" FILES ${Serialize})

file(GLOB Sampler ${CMAKE_CURRENT_SOURCE_DIR}/*.h*)

set(SOURCE_FILES
		${Animation}
		${Render}
		${Serialize}
		${Sampler}
		SampleBrowser.cpp
		main-windows.cpp
		)

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/../"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Plugin"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern/Bullet-2.82/src"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern/mono/Inlude"
)


link_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern_vs/Win32/Debug"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern/vulkan/lib"
)



if(WIN32)
	add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES})

	target_link_libraries(${PROJECT_NAME} Common Engine VulkanRender Animation BulletPhysics Application)

	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	add_definitions(-DWIN32)

   set_target_properties(${PROJECT_NAME}  PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
   set_target_properties(${PROJECT_NAME}  PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
   set_target_properties(${PROJECT_NAME}  PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
   set_target_properties(${PROJECT_NAME}  PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_CONSOLE")
   set_target_properties(${PROJECT_NAME}  PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
   set_target_properties(${PROJECT_NAME}  PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")

else(WIN32)
	add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

	target_link_libraries(${PROJECT_NAME} Common Engine VulkanRender Animation BulletPhysics Application)
endif(WIN32)

