project(FreeImage)

file(GLOB_RECURSE DeprecationManager ${CMAKE_CURRENT_SOURCE_DIR}/DeprecationManager/*.cpp)
source_group("DeprecationManager" FILES ${DeprecationManager})

file(GLOB_RECURSE FreeImage ${CMAKE_CURRENT_SOURCE_DIR}/FreeImage/*.cpp)
source_group("FreeImage" FILES ${FreeImage})

file(GLOB_RECURSE FreeImageToolkit ${CMAKE_CURRENT_SOURCE_DIR}/FreeImageToolkit/*.cpp)
source_group("FreeImageToolkit" FILES ${FreeImageToolkit})

file(GLOB_RECURSE Metadata ${CMAKE_CURRENT_SOURCE_DIR}/Metadata/*.cpp)
source_group("Metadata" FILES ${Metadata})

file(GLOB_RECURSE LibJPEG ${CMAKE_CURRENT_SOURCE_DIR}/LibJPEG/*.c)
source_group("LibJPEG" FILES ${LibJPEG})

file(GLOB_RECURSE LibPNG ${CMAKE_CURRENT_SOURCE_DIR}/LibPNG/*.c)
source_group("LibPNG" FILES ${LibPNG})

file(GLOB_RECURSE LibMNG ${CMAKE_CURRENT_SOURCE_DIR}/LibMNG/*.c)
source_group("LibMNG" FILES ${LibMNG})

file(GLOB_RECURSE LibOpenJPEG ${CMAKE_CURRENT_SOURCE_DIR}/LibOpenJPEG/*.c)
source_group("LibOpenJPEG" FILES ${LibOpenJPEG})

file(GLOB_RECURSE LibRawLite ${CMAKE_CURRENT_SOURCE_DIR}/LibRawLite/*.cpp)
source_group("LibRawLite" FILES ${LibRawLite})

file(GLOB_RECURSE LibTIFF ${CMAKE_CURRENT_SOURCE_DIR}/LibTIFF/*.c)
source_group("LibTIFF" FILES ${LibTIFF})

file(GLOB_RECURSE OpenEXR ${CMAKE_CURRENT_SOURCE_DIR}/OpenEXR/*.cpp)
source_group("OpenEXR" FILES ${OpenEXR})


add_library(${PROJECT_NAME} STATIC 
	${DeprecationManager} 
	${FreeImage} 
	${FreeImageToolkit}
	${Metadata}
	${LibJPEG} 
	${LibPNG} 
	${LibMNG} 
	${LibOpenJPEG}
	${LibTIFF} 
	${OpenEXR} 
	)

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/"
	"${CMAKE_CURRENT_SOURCE_DIR}/DeprecationManager"
	"${CMAKE_CURRENT_SOURCE_DIR}/OpenEXR"
	"${CMAKE_CURRENT_SOURCE_DIR}/OpenEXR/Half"
	"${CMAKE_CURRENT_SOURCE_DIR}/OpenEXR/Iex"
	"${CMAKE_CURRENT_SOURCE_DIR}/OpenEXR/IlmImf"
	"${CMAKE_CURRENT_SOURCE_DIR}/OpenEXR/Imath"
	"${CMAKE_CURRENT_SOURCE_DIR}/OpenEXR/IlmThread"
	"${CMAKE_CURRENT_SOURCE_DIR}/LibRawLite"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../zlib-1.2.3"
)

if (WIN32)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

	add_definitions(-DWIN32)
	add_definitions(-DOPJ_STATIC)
	add_definitions(-DFREEIMAGE_LIB)
	add_definitions(-DLIBRAW_NODLL)
endif()
