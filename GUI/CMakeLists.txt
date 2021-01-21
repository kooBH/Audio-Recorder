cmake_minimum_required(VERSION 3.16.0)

# CMAKE version of Ubuntu 16.04 is 3.5.
# cmake_minimum_required(VERSION 3.5)

project(Recorder VERSION 1.0.0 LANGUAGES CXX)

# Qt6 standard  
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

# Not gonna use them
#set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)

# Qt6 is not in system path. Must be built in standalone project.
# find_package(Qt6 COMPONENTS Widgets REQUIRED)

list(APPEND SRC
   src/main.cpp
   src/KSignal.h
   src/KSlot.h 
)

## QtFramework ##
if(UNIX AND NOT APPLE)
  list(APPEND LINKLIBS
    ${PROJECT_SOURCE_DIR}/lib/libQt5Widgets.so.5
    ${PROJECT_SOURCE_DIR}/lib/libQt5Core.so.5
    ${PROJECT_SOURCE_DIR}/lib/libQt5Gui.so.5
	
  	### X11 ###
    ${PROJECT_SOURCE_DIR}/lib/libQt5XcbQpa.so.5
    ${PROJECT_SOURCE_DIR}/lib/platforms/libqxcb.so

    ### Input ###
    ${PROJECT_SOURCE_DIR}/lib/platforminputcontexts/libcomposeplatforminputcontextplugin.so
    ${PROJECT_SOURCE_DIR}/lib/platforminputcontexts/libibusplatforminputcontextplugin.so
	)
  list(APPEND COMPILE_OPTION
      -fPIC
    )
elseif(WIN32)

	# aren't DLLs enough?
	#list(APPEND LINKLIBS
	#	${PROJECT_SOURCE_DIR}/lib/Qt5Widgets.lib
	#	${PROJECT_SOURCE_DIR}/lib/Qt5Core.lib
	#	${PROJECT_SOURCE_DIR}/lib/Qt5Gui.lib
	# )
endif()

add_executable(${PROJECT_NAME} ${SRC})
target_include_directories(${PROJECT_NAME}	PRIVATE	${INCL}  )
target_link_libraries(${PROJECT_NAME} ${LINKLIBS})
target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_OPTION} )