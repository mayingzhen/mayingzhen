project(Engine)

message(STATUS "Configuring Engine")

#function(create_source_dir dir)
#	file(GLOB_RECURSE ${dir} ${CMAKE_CURRENT_SOURCE_DIR}/ImageData/*.h*)
#	source_group(${dir} FILES ${dir})
#endfunction()

#create_source_dir(ImageData)

file(GLOB_RECURSE ImageData ${CMAKE_CURRENT_SOURCE_DIR}/ImageData/*.h*)
source_group("ImageData" FILES ${ImageData})

file(GLOB_RECURSE Material ${CMAKE_CURRENT_SOURCE_DIR}/Material/*.h*)
source_group("Material" FILES ${Material})

file(GLOB_RECURSE Renderable ${CMAKE_CURRENT_SOURCE_DIR}/Renderable/*.h*)
source_group("Renderable" FILES ${Renderable})

file(GLOB_RECURSE RenderScheme ${CMAKE_CURRENT_SOURCE_DIR}/RenderScheme/*.h*)
source_group("RenderScheme" FILES ${RenderScheme})

file(GLOB_RECURSE RenderSystem ${CMAKE_CURRENT_SOURCE_DIR}/RenderSystem/*.h*)
source_group("RenderSystem" FILES ${RenderSystem})

file(GLOB_RECURSE Resource ${CMAKE_CURRENT_SOURCE_DIR}/Resource/*.h*)
source_group("Resource" FILES ${Resource})

file(GLOB_RECURSE RTTI ${CMAKE_CURRENT_SOURCE_DIR}/RTTI/*.h*)
source_group("RTTI" FILES ${RTTI})

file(GLOB_RECURSE Scene ${CMAKE_CURRENT_SOURCE_DIR}/Scene/*.h*)
source_group("Scene" FILES ${Scene})

file(GLOB_RECURSE Scene ${CMAKE_CURRENT_SOURCE_DIR}/Scene/*.h*)
source_group("Scene" FILES ${Scene})

file(GLOB_RECURSE Thread ${CMAKE_CURRENT_SOURCE_DIR}/Thread/*.h*)
source_group("Thread" FILES ${Thread})

file(GLOB_RECURSE Time ${CMAKE_CURRENT_SOURCE_DIR}/Time/*.h*)
source_group("Time" FILES ${Time})

file(GLOB Engine ${CMAKE_CURRENT_SOURCE_DIR}/*.h*)


set(SOURCE_FILES
		${ImageData}
		${Material}
		${Renderable}
		${RenderScheme}
		${RenderSystem}
		${Resource}
		${RTTI}
		${Scene}
		${Thread}
		${Time}
		${Engine}
		Module.cpp 
		Export.def)

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/../"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern/zziplib-0.13.57"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern/FreeImage-3.13.1/Source"
)




if (WIN32)
	add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

	target_link_libraries(${PROJECT_NAME} Common FreeImage zziplib zlib)
	
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj")
else (WIN32)
	add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

	target_link_libraries(${PROJECT_NAME} Common FreeImage zziplib zlib log android)
endif()




