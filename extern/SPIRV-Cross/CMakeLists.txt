project(SPIRV-Cross)


add_library(${PROJECT_NAME} STATIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/spirv_msl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/spirv_hlsl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/spirv_glsl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/spirv_cross.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/spirv_cpp.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/spirv_cfg.cpp
  )

if (WIN32)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()
