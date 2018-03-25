project(glslang)

file(GLOB_RECURSE SPIRV ${CMAKE_CURRENT_SOURCE_DIR}/SPIRV/*.cpp)
source_group("SPIRV" FILES ${SPIRV})

file(GLOB_RECURSE OGLCompilersDLL ${CMAKE_CURRENT_SOURCE_DIR}/OGLCompilersDLL/*.cpp)
source_group("OGLCompilersDLL" FILES ${OGLCompilersDLL})

file(GLOB_RECURSE hlsl ${CMAKE_CURRENT_SOURCE_DIR}/hlsl/*.cpp)
source_group("hlsl" FILES ${hlsl})

file(GLOB_RECURSE GenericCodeGen ${CMAKE_CURRENT_SOURCE_DIR}/glslang/GenericCodeGen/*.cpp)
source_group("GenericCodeGen" FILES ${GenericCodeGen})

file(GLOB_RECURSE MachineIndependent ${CMAKE_CURRENT_SOURCE_DIR}/glslang/MachineIndependent/*.cpp)
source_group("MachineIndependent" FILES ${MachineIndependent})

if (WIN32)
file(GLOB_RECURSE OSDependent ${CMAKE_CURRENT_SOURCE_DIR}/glslang/OSDependent/Windows/ossource.cpp)
source_group("OSDependent" FILES ${OSDependent})
else(WIN32)
file(GLOB_RECURSE OSDependent ${CMAKE_CURRENT_SOURCE_DIR}/glslang/OSDependent/Unix/ossource.cpp)
source_group("OSDependent" FILES ${OSDependent})
endif()

add_library(${PROJECT_NAME} STATIC 
    ${SPIRV} 
    ${OGLCompilersDLL} 
    ${hlsl}
    ${GenericCodeGen} 
    ${MachineIndependent} 
    ${OSDependent}
    )

add_definitions(-DENABLE_HLSL)

if (WIN32)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

