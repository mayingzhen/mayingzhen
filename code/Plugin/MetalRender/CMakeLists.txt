project(MetalRender)

message(STATUS "Configuring MetalRender")

file(GLOB metal_h ${CMAKE_CURRENT_SOURCE_DIR}/*.h*)
file(GLOB metal_mm ${CMAKE_CURRENT_SOURCE_DIR}/*.mm)

set(SOURCE_FILES
		${metal_h}
		${metal_mm}
		)

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/../"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/vulkan"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../code/Application/Android"
)

link_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern_vs/Win32/Debug"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/vulkan/lib"
)


if(WIN32)
   	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

	add_definitions(-DWIN32)
	add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)
	add_definitions(-DUSRDLL)
	add_definitions(-DVULKANRENDER_EXPORTS)

	add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
	
	target_link_libraries(${PROJECT_NAME} Engine Common glslang SPIRV-Cross vulkan-1.lib)

else(WIN32)
	add_definitions(-D__STDC_LIMIT_MACROS)
	add_definitions(-DVK_NO_PROTOTYPES)
	add_definitions(-DVK_USE_PLATFORM_ANDROID_KHR)

	add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

	target_link_libraries(${PROJECT_NAME} Engine Common glslang SPIRV-Cross)

endif(WIN32)

target_precompile_headers(${PROJECT_NAME} PUBLIC stdafx.h)





