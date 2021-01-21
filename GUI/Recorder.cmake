### Simple Recording Program ###

list(APPEND SRC
    src/main_recorder.cpp
    src/K/KRecorder.h
    src/K/KRecorder.cpp  
		

lib/RtAudio/RtAudio.cpp
lib/RtAudio/RtAudio.h
    src/util/RT_Input.h
    src/util/RT_Input.cpp     
    src/util/WAV/WAV.h
    src/util/WAV/WAV.cpp   
  )

##############
###   QT   ###
##############
  list(APPEND SRC
  #### Base Widget ####

	src/K/KDef.h
	src/K/KComboBox.h
	src/K/KLineEdit.h
	src/K/RealTimeSwitch.h
	src/K/KClickableLabel.h

  src/K/BorderLayout.h
	src/K/BorderLayout.cpp

	src/K/Armatita_switch.h
	src/K/Armatita_switch.cpp
	src/K/KModuleSwitch.h

	src/K/IMAN4F_Switch.h
	src/K/IMAN4F_Switch.cpp
	src/K/IMAN4F_style.h

	src/K/KRecordPlot_rec.cpp
	src/K/KRecordPlot_rec.h
	
  #### Widget Module ###
  ${PROJECT_SOURCE_DIR}/src/K/KInput.h
  ${PROJECT_SOURCE_DIR}/src/K/KParam.h
    )

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

  list(APPEND COMPILE_OPTION -fPIC)
elseif(WIN32)
  list(APPEND LINKLIBS
    debug ${PROJECT_SOURCE_DIR}/lib/Qt5Cored.lib optimized ${PROJECT_SOURCE_DIR}/lib/Qt5Core.lib
    debug ${PROJECT_SOURCE_DIR}/lib/Qt5Guid.lib optimized ${PROJECT_SOURCE_DIR}/lib/Qt5Gui.lib
    debug ${PROJECT_SOURCE_DIR}/lib/Qt5Widgetsd.lib optimized ${PROJECT_SOURCE_DIR}/lib/Qt5Widgets.lib		
    )
  #set(QTDEBUG_LINKLIBS)
  #list(APPEND QTDEBUG_LINKLIBS
  #	#${PROJECT_SOURCE_DIR}/lib/Qt5Cored.lib
  #	#${PROJECT_SOURCE_DIR}/lib/Qt5Guid.lib
  #	#${PROJECT_SOURCE_DIR}/lib/Qt5Widgets.lib		
  #)

  list(APPEND COMPILE_OPTION /D_QT5)
 elseif(APPLE)
    set(Qt5_DIR ~/Qt/5.12.3/clang_64/lib/cmake/Qt5)
    find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
    list(APPEND LINKLIBS Qt5::Core Qt5::Gui Qt5::Widgets)

endif(UNIX AND NOT APPLE)

 
 
#################
### verdigris ###
#################

 list(APPEND SRC
    lib/verdigris/wobjectimpl.h
    lib/verdigris/wobjectdefs.h
    )

##############
###        ###
##############

list(APPEND INCL
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/src/audio
	${CMAKE_SOURCE_DIR}/lib

   lib/Qt5
   lib/Qt5/QtCore
   lib/Qt5/QtGui
   lib/Qt5/QtWidgets

)

list(APPEND COMPILE_OPTION
-D_CONFIG_JSON="../config/config.json"
-D_RECORDER
)

add_executable(recorder ${SRC})
target_include_directories(recorder
	PRIVATE
  ${INCL}
  )
target_link_libraries(recorder ${LINKLIBS})
target_compile_options(recorder PRIVATE ${COMPILE_OPTION} )
set_target_properties(recorder PROPERTIES
    CXX_STANDARD 14
)
