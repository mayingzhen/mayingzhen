project(Common)

message(STATUS "Configuring Common")

file(GLOB_RECURSE Math ${CMAKE_CURRENT_SOURCE_DIR}/Math/*.h*)
source_group("Math" FILES ${Math})

file(GLOB_RECURSE RapidXml ${CMAKE_CURRENT_SOURCE_DIR}/RapidXml/*.h*)
source_group("RapidXml" FILES ${RapidXml})

file(GLOB Common ${CMAKE_CURRENT_SOURCE_DIR}/*.h*)

set(SOURCE_FILES
		${Math}
		${RapidXml}
		${Common}
		Module.cpp
		)

if (WIN32)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
