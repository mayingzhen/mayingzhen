cmake_minimum_required(VERSION 2.6)

set_property(GLOBAL PROPERTY USE_FOLDERS On)

# 配置类型只支持Debug和带调试信息的Release
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

set(CMAKE_MODULE_PATH
  "${CMAKE_SOURCE_DIR}/CMake"
)

# 工程名为NeoX
project(myz)

add_subdirectory(extern)

add_subdirectory(code)

add_dependencies(Engine zlib zziplib FreeImage)
add_dependencies(VulkanRender glslang SPIRV-Cross)
add_dependencies(BulletPhysics Bullet)

