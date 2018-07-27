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
project(myz_android)

add_subdirectory(extern)

add_subdirectory(code)

add_dependencies(engine zlib zziplib FreeImage)
add_dependencies(VulkanRender glslang SPIRV-Cross)
add_dependencies(BulletPhysics Bullet)

