cmake_minimum_required(VERSION 2.8)

# 配置类型只支持Debug和带调试信息的Release
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

set(CMAKE_MODULE_PATH
  "${CMAKE_SOURCE_DIR}/CMake"
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(extern)

add_subdirectory(zlib-1.2.3)

add_subdirectory(zziplib-0.13.57)

add_subdirectory(glslang)

add_subdirectory(SPIRV-Cross)

add_subdirectory(Bullet-2.82/src)

add_subdirectory(FreeImage-3.13.1/Source)

add_subdirectory(imgui)

#if (WIN32)
	set_target_properties(zlib zziplib glslang SPIRV-Cross Bullet FreeImage
		PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}"
		RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}"
		PDB_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}"
		)
#endif()




