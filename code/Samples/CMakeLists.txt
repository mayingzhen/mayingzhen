project(SampleBrowser)

message(STATUS "Configuring SampleBrowser")

set(SOURCE_FILES
		SampleBrowser.cpp
		SampleBrowser.h
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

	target_link_libraries(${PROJECT_NAME} common engine VulkanRender MonoScript Animation BulletPhysics Application)

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

	target_link_libraries(${PROJECT_NAME} common engine VulkanRender Animation BulletPhysics Application)
endif(WIN32)

