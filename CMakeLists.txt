cmake_minimum_required(VERSION 3.4)
project(FRE_Visualizer)

FIND_PACKAGE(Qt5Widgets)
FIND_PACKAGE(Qt5Gui)
FIND_PACKAGE(OpenSceneGraph REQUIRED COMPONENTS osgDB osgGA osgUtil osgViewer osgText)

add_subdirectory(libs/FRE_Slicer)
INCLUDE_DIRECTORIES( ${OPENSCENEGRAPH_INCLUDE_DIRS} FRE_Slicer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED 1)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTORCC ON)

SET(MYSOURCE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindowform.ui
    osgwidget.h
    osgwidget.cpp
    wireframe.cpp
    wireframe.h
    printshape.cpp
    printshape.h
    )

add_executable(${PROJECT_NAME} ${MYSOURCE})

target_link_libraries(${PROJECT_NAME}
    ${OPENSCENEGRAPH_LIBRARIES}
    Qt5::Widgets
    Qt5::Gui
    FRE_Slicer)

IF (NOT TARGET gtest)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(libs/googletest)
ENDIF()

set(TEST_NAME ${PROJECT_NAME}_UnitTest)
add_executable(${TEST_NAME} layercalculations_unittests.cpp)
target_link_libraries(${TEST_NAME} gtest gtest_main)
