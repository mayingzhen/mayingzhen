project(Application)

message(STATUS "Configuring Application")

file(GLOB_RECURSE Android ${CMAKE_CURRENT_SOURCE_DIR}/Android/*.h*)
source_group("Android" FILES ${Android})

file(GLOB_RECURSE Input ${CMAKE_CURRENT_SOURCE_DIR}/Input/*.h*)
source_group("Input" FILES ${Input})

file(GLOB_RECURSE iOS_h ${CMAKE_CURRENT_SOURCE_DIR}/iOS/*.h*)
file(GLOB_RECURSE iOS_mm ${CMAKE_CURRENT_SOURCE_DIR}/iOS/*.mm)
source_group("iOS" FILES ${iOS_h} ${iOiOS_mm})

file(GLOB_RECURSE Win ${CMAKE_CURRENT_SOURCE_DIR}/Win/*.h*)
source_group("Win" FILES ${Win})

file(GLOB Application ${CMAKE_CURRENT_SOURCE_DIR}/*.h*)

set(SOURCE_FILES
		${Android}
		${Input}
		${iOS_h}
		${iOS_mm}
		${Win}
		${Application}
		Module.cpp
		)

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/../"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extern/imgui"
)


if (WIN32)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

	add_definitions(-DWIN32)

	add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
else (WIN32)
	add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
endif()


target_precompile_headers(${PROJECT_NAME} PUBLIC stdafx.h)

target_link_libraries(${PROJECT_NAME} Common Engine imgui)

add_definitions(-D_USRDLL)
add_definitions(-DAPP_EXPORTS)
