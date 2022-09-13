cmake_minimum_required(VERSION 3.5)

project(test2 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH $ENV{QTDIR})


find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5Widgets REQUIRED QUIET)
set(VTK_DIR $ENV{VTK_DIR})
set(ITK_DIR $ENV{ITK_DIR})

qt5_wrap_ui(UISrcs ${UI_FILES})
#======================= INCLUSION OF VTK ======================#
set(VTK_DIR $ENV{VTK_DIR})
FIND_PACKAGE(VTK REQUIRED)
INCLUDE( ${VTK_USE_FILE} )
#======================= INCLUSION OF ITK ======================#
set(ITK_DIR $ENV{ITK_DIR})
FIND_PACKAGE(ITK REQUIRED)
INCLUDE( ${ITK_USE_FILE} )


if(ANDROID)
  add_library(test2 SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
  )
else()
  add_executable(test2
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
  )
endif()

target_link_libraries(test2 PUBLIC ${ITK_LIBRARIES} ${VTK_LIBRARIES} ${QT_LIBRARIES})
