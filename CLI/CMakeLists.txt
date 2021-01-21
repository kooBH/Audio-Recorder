cmake_minimum_required(VERSION 3.5.0)
# CMAKE version of Ubuntu 16.04 is 3.5.
# cmake_minimum_required(VERSION 3.5)

project(Recorder VERSION 1.0.0 LANGUAGES CXX)

# Qt6 is not in system path. Must be built in standalone project.
# find_package(Qt6 COMPONENTS Widgets REQUIRED)



list(APPEND INCL
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/../lib/json_for_modern_cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../lib/RtAudioWrapper
	${CMAKE_CURRENT_SOURCE_DIR}/../lib/WAV
)
	
if(UNIX AND NOT APPLE)
list(APPEND COMPILE_OPTION
		# -O3 : maximize performace
		# -O2 : maximize performance as long as not increasing size of executable
		# -O1 : minimize compile time and others are secondary
		# -Os : minimize size of executable
		-O3
	)
else()
	list(APPEND COMPILE_OPTION
		# /O2 - fastest code in the majority of cases
		/O2
	)
endif()


include(${CMAKE_CURRENT_SOURCE_DIR}/../lib/RtAudioWrapper/RtAudio.cmake)

# Build AudioProbe first
include(${CMAKE_CURRENT_SOURCE_DIR}/../lib/RtAudioWrapper/AudioProbe.cmake)

# then append sources for Recorder
list(APPEND SRC 
	src/main.cpp
	src/Recorder.h
)

add_executable(${PROJECT_NAME} src/main.cpp ${SRC})
target_include_directories(${PROJECT_NAME}	PRIVATE	${INCL}  )
target_link_libraries(${PROJECT_NAME} ${LINKLIBS})
target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_OPTION} )