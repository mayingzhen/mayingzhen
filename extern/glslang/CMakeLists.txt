project(glslang)

if (WIN32)
	file(GLOB_RECURSE glslang 
		${CMAKE_CURRENT_SOURCE_DIR}/glslang/GenericCodeGen/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/glslang/MachineIndependent/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/glslang/OSDependent/Windows/*.cpp
		)
else ()
	file(GLOB_RECURSE glslang 
		${CMAKE_CURRENT_SOURCE_DIR}/glslang/GenericCodeGen/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/glslang/MachineIndependent/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/glslang/OSDependent/Unix/*.cpp
		)
endif()
source_group("glslang" FILES ${glslang})

file(GLOB_RECURSE OGLCompilersDLL ${CMAKE_CURRENT_SOURCE_DIR}/OGLCompilersDLL/*.cpp)
source_group("OGLCompilersDLL" FILES ${OGLCompilersDLL})

file(GLOB_RECURSE hlsl ${CMAKE_CURRENT_SOURCE_DIR}/hlsl/*.cpp)
source_group("hlsl" FILES ${hlsl})

file(GLOB_RECURSE SPIRV ${CMAKE_CURRENT_SOURCE_DIR}/SPIRV/*.cpp)
source_group("SPIRV" FILES ${SPIRV})

add_library(${PROJECT_NAME} STATIC ${glslang} ${OGLCompilersDLL} ${hlsl} ${SPIRV})


add_definitions(-DAMD_EXTENSIONS)
add_definitions(-DNV_EXTENSIONS)
add_definitions(-DENABLE_HLSL)



include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/"
	"${CMAKE_CURRENT_SOURCE_DIR}/../"
)

if (WIN32)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "extern") 