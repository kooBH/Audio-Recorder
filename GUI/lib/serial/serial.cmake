#########################################
#                                       #
#        Serial Port Driver             #
#   https://github.com/wjwwood/serial   #
#                                       #
#########################################

if(APPLE)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(FOUNDATION_LIBRARY Foundation)
endif()

## Sources
list(APPEND SRC
    ${CMAKE_CURRENT_LIST_DIR}/serial.cc
    ${CMAKE_CURRENT_LIST_DIR}/serial.h
    ${CMAKE_CURRENT_LIST_DIR}/v8stdint.h
    )
if(APPLE)
    # If OSX
    list(APPEND SRC ${CMAKE_CURRENT_LIST_DIR}/impl/unix.cc)
    list(APPEND SRC ${CMAKE_CURRENT_LIST_DIR}/impl/list_ports/list_ports_osx.cc)
elseif(UNIX)
    # If unix
    list(APPEND SRC ${CMAKE_CURRENT_LIST_DIR}/impl/unix.cc)
    list(APPEND SRC ${CMAKE_CURRENT_LIST_DIR}/impl/list_ports/list_ports_linux.cc)
else()
    # If windows
    set(SRC ${SRC} ${CMAKE_CURRENT_LIST_DIR}/impl/win.cc )
    set(SRC ${SRC} ${CMAKE_CURRENT_LIST_DIR}/impl/list_ports/list_ports_win.cc )
endif()

## Add serial library
if(APPLE)
    set(LINKLIBS ${LINKLIBS} ${FOUNDATION_LIBRARY} ${IOKIT_LIBRARY} )
elseif(UNIX)
    set(LINKLIBS ${LINKLIBS} rt pthread )
else()
    set(LINKLIBS ${LINKLIBS} setupapi )
endif()
