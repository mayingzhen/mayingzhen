project(Samples)

message(STATUS "Configuring Samples")

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
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern/zziplib-0.13.57/msvc8"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern/FreeImage-3.13.1/Source/FreeImageLib/Debug"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern/Bullet-2.82/build/vs2008/Lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern/vulkan/lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern/glslang/glslang/Debug/"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern/glslang/SPIRV/Debug/"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern/glslang/OGLCompilersDLL/Debug/"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern/glslang/hlsl/Debug/"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern/glslang/glslang/OSDependent/Windows/Debug/"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern/SPIRV-Cross/Debug/"
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} common engine VulkanRender MonoScript Animation BulletPhysics Application)

add_definitions(-DWIN32)

if(WIN32)
   set_target_properties(${PROJECT_NAME}  PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
   set_target_properties(${PROJECT_NAME}  PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
   set_target_properties(${PROJECT_NAME}  PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
   set_target_properties(${PROJECT_NAME}  PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_CONSOLE")
   set_target_properties(${PROJECT_NAME}  PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
   set_target_properties(${PROJECT_NAME}  PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
endif(WIN32)