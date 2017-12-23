project(VulkanRender)

message(STATUS "Configuring VulkanRender")

set(SOURCE_FILES
		Module.h
		Module.cpp
		stdafx.h
		stdafx.cpp
		)

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/../"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/vulkan"
)

link_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/zziplib-0.13.57/msvc8"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/FreeImage-3.13.1/Source/FreeImageLib/Debug"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/vulkan/lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/glslang/glslang/Debug/"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/glslang/SPIRV/Debug/"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/glslang/OGLCompilersDLL/Debug/"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/glslang/hlsl/Debug/"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/glslang/glslang/OSDependent/Windows/Debug/"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/SPIRV-Cross/Debug/"
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

set(EXTERN_LINK_LIB
vulkan-1.lib
glslangd.lib
SPIRVd.lib
OGLCompilerd.lib
HLSLd.lib
OSDependentd.lib
spirv-cross-core.lib
spirv-cross-glsl.lib
spirv-cross-hlsl.lib
spirv-cross-msl.lib
)

target_link_libraries(${PROJECT_NAME} common engine ${EXTERN_LINK_LIB})

add_definitions(-DWIN32)
add_definitions(-DUSRDLL)
add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)
add_definitions(-DVULKANRENDER_EXPORTS)


