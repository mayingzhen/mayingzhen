cmake_minimum_required(VERSION 2.8)
#set(CMAKE_LEGACY_CYGWIN_WIN32 0)
#set (CMAKE_USER_MAKE_RULES_OVERRIDE "${CMAKE_CURRENT_LIST_DIR}/CMake/NeoXOverrideOptions.cmake")

# Use relative paths
# This is mostly to reduce path size for command-line limits on windows
#if(WIN32)
  # This seems to break Xcode projects so definitely don't enable on Apple builds
#  set(CMAKE_USE_RELATIVE_PATHS true)
#  set(CMAKE_SUPPRESS_REGENERATION true)
#endif()

# 配置类型只支持Debug和带调试信息的Release
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

set(CMAKE_MODULE_PATH
  "${CMAKE_SOURCE_DIR}/CMake"
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(extern)

#include(NeoXCommon)

#include(NeoXMinimum)

add_subdirectory(zlib-1.2.3)

add_subdirectory(zziplib-0.13.57)

add_subdirectory(glslang)

add_subdirectory(SPIRV-Cross)

add_subdirectory(Bullet-2.82/src)

add_subdirectory(FreeImage-3.13.1/Source)

#if (WIN32)
	set_target_properties(zlib zziplib glslang SPIRV-Cross Bullet FreeImage
		PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}"
		RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}"
		PDB_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}"
		)
#endif()

### glsl
#set_target_properties(glslang PROPERTIES FOLDER glslang)
#set_target_properties(OGLCompiler PROPERTIES FOLDER glslang)
#set_target_properties(OSDependent PROPERTIES FOLDER glslang)
#set_target_properties(SPIRV PROPERTIES FOLDER glslang)
#set_target_properties(SPVRemapper PROPERTIES FOLDER glslang)
#set_target_properties(HLSL PROPERTIES FOLDER glslang)

### SPIRV-Cross
#set_target_properties(spirv-cross PROPERTIES FOLDER SPIRV-Cross)
#set_target_properties(spirv-cross-core PROPERTIES FOLDER SPIRV-Cross)
#set_target_properties(spirv-cross-cpp PROPERTIES FOLDER SPIRV-Cross)
#set_target_properties(spirv-cross-glsl PROPERTIES FOLDER SPIRV-Cross)
#set_target_properties(spirv-cross-hlsl PROPERTIES FOLDER SPIRV-Cross)
#set_target_properties(spirv-cross-msl PROPERTIES FOLDER SPIRV-Cross)


