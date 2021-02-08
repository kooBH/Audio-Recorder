################################################################
# CMake for Recorder Source files                              #
# 															   #
# Note : 'config.json' file must exist in '../config.json'     #
################################################################

# Sources to be compiled by MOC
list(APPEND SRC_Qt_MOC
	${CMAKE_CURRENT_LIST_DIR}/src/KRecordPlot.h
	${CMAKE_CURRENT_LIST_DIR}/src/KRecorderControl.h
	${CMAKE_CURRENT_LIST_DIR}/src/KRecorder.h
)

list(APPEND SRC_Qt
	${CMAKE_CURRENT_LIST_DIR}/src/KInput.h
	${CMAKE_CURRENT_LIST_DIR}/src/KParam.h
	${CMAKE_CURRENT_LIST_DIR}/src/KRecorder.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/KRecorderControl.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/KRecordPlot.cpp
	
)

list(APPEND SRC
	${CMAKE_CURRENT_LIST_DIR}/src/JsonConfig.h
	${CMAKE_CURRENT_LIST_DIR}/src/Config.h
	)
	
list(APPEND INCL
	${CMAKE_CURRENT_LIST_DIR}/src
	${CMAKE_CURRENT_LIST_DIR}/lib/serial
	${CMAKE_CURRENT_LIST_DIR}/../lib/json
)

include(${CMAKE_CURRENT_LIST_DIR}/lib/serial/serial.cmake)
	
## PATH
list(APPEND COMPILE_OPTION
  -D_CONFIG_JSON="../config.json"
)  


