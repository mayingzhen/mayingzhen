cmake_minimum_required(VERSION 2.8)

# 配置类型只支持Debug和带调试信息的Release
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

set(CMAKE_MODULE_PATH
  "${CMAKE_SOURCE_DIR}/CMake"
)

project(myz)

add_subdirectory(Common)

add_subdirectory(Engine)

add_subdirectory(Plugin/Animation)

if (IOS)
	add_subdirectory(Plugin/MetalRender)
else()
	add_subdirectory(Plugin/VulkanRender)
endif()

add_subdirectory(Plugin/BulletPhysics)

if (WIN32)
	#add_subdirectory(Plugin/MonoScript)
	#add_dependencies(MonoScript Common Engine)
endif()

add_subdirectory(Application)

add_subdirectory(Samples)

add_dependencies(Engine Common)

add_dependencies(Animation Common Engine)

if (IOS)
	add_dependencies(MetalRender Common Engine)
else()
	add_dependencies(VulkanRender Common Engine)
endif()

add_dependencies(BulletPhysics Common Engine)

add_dependencies(Application Common Engine)

add_dependencies(SampleBrowser Application Common Engine)

if (WIN32)
	set_target_properties(Common Engine Application BulletPhysics VulkanRender Animation SampleBrowser
		PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/win32"
		RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/win32"
		PDB_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/win32"
		)
endif()
