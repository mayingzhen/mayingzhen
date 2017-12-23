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

# 工程名为NeoX
project(myz)

#include(NeoXCommon)

#include(NeoXMinimum)

add_subdirectory(Common)

add_subdirectory(Engine)

add_subdirectory(Plugin/Animation)

add_subdirectory(Plugin/VulkanRender)

add_subdirectory(Plugin/BulletPhysics)

add_subdirectory(Plugin/MonoScript)

add_subdirectory(Application)

add_subdirectory(Samples)

add_dependencies(engine common)

add_dependencies(Animation common engine)

add_dependencies(VulkanRender common engine)

add_dependencies(BulletPhysics common engine)

add_dependencies(MonoScript common engine)

add_dependencies(Application common engine)

add_dependencies(Samples Application common engine)

if (WIN32)
	set_target_properties(common engine Application MonoScript BulletPhysics VulkanRender Animation Samples
		PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/win32"
		RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/win32"
		PDB_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/win32"
		)
endif()