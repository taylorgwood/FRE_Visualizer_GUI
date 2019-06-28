cmake_minimum_required(VERSION 3.4)
project(EDP_Visualizer)

FIND_PACKAGE(Qt5Widgets)
FIND_PACKAGE(Qt5Gui)
FIND_PACKAGE(OpenSceneGraph REQUIRED COMPONENTS osgDB osgGA osgUtil osgViewer osgText)

SET(MY_LIBRARY libs/FRE_Slicer)
add_subdirectory(${MY_LIBRARY})
INCLUDE_DIRECTORIES( ${OPENSCENEGRAPH_INCLUDE_DIRS} ${MY_LIBRARY})

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
    )

SET(MYRESOURCES icons/icons.qrc)

add_executable(${PROJECT_NAME} WIN32
    ${MYSOURCE}
    ${MYRESOURCES}
    )

target_link_libraries(${PROJECT_NAME}
    ${OPENSCENEGRAPH_LIBRARIES}
    Qt5::Widgets
    Qt5::Gui
    FRE_Slicer)

#--------------------------------------------------------------------------------
# Now the installation stuff below
#--------------------------------------------------------------------------------

#install(TARGETS ${MY_LIBRARY}
#  ARCHIVE
#  DESTINATION lib)
#install(TARGETS ${PROJECT_NAME}
#  RUNTIME
#  DESTINATION bin)
#install(FILES ${MYSOURCE}
#  DESTINATION include)



#--------------------------------------------------------------------------------
# This section if for including Qt Libraries
#--------------------------------------------------------------------------------

##--------------------------------------------------------------------------------
## Find the location of the Qt Libraries and set the location in QT_LIBRARY_DIR
## and QT_PLUGINS_DIR
#GET_TARGET_PROPERTY(_loc Qt5::qmake LOCATION)
#GET_FILENAME_COMPONENT(_qmake_dir ${_loc} DIRECTORY)
#GET_FILENAME_COMPONENT(_qt_dir ${_qmake_dir} DIRECTORY)
#SET(QT_LIBRARY_DIR ${_qt_dir}/lib)
#SET(QT_PLUGINS_DIR ${_qt_dir}/plugins)

##--------------------------------------------------------------------------------
## Determine the where to install the Qt plugins and qtconf file and save the
## location in PLUGIN_DEST_DIR and QTCONF_DEST_DIR
#SET(PLUGIN_DEST_DIR bin)
#SET(QTCONF_DEST_DIR bin)
#SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/${PROJECT_NAME}")
#IF(APPLE)
#  SET(PLUGIN_DEST_DIR ${PROJECT_NAME}.app/Contents/MacOS)
#  SET(QTCONF_DEST_DIR ${PROJECT_NAME}.app/Contents/Resources)
#  SET(APPS "\${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}.app")
#ENDIF(APPLE)
#IF(WIN32)
#  SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/${PROJECT_NAME}.exe")
#ENDIF(WIN32)

##--------------------------------------------------------------------------------
## Install the application, on Apple, the bundle is at the root of the
## install tree, and on other platforms it'll go into the bin directory.
#INSTALL(TARGETS ${PROJECT_NAME}
#    BUNDLE DESTINATION . COMPONENT Runtime
#    RUNTIME DESTINATION bin COMPONENT Runtime
#    )

##--------------------------------------------------------------------------------
## Install needed Qt plugins by copying directories from the qt installation
## One can cull what gets copied by using 'REGEX "..." EXCLUDE'
#INSTALL(DIRECTORY "${QT_PLUGINS_DIR}/platforms" DESTINATION ${PLUGIN_DEST_DIR}/plugins COMPONENT Runtime)

##--------------------------------------------------------------------------------
## install a qt.conf file
## this inserts some cmake code into the install script to write the file
#INSTALL(CODE "
#    file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${QTCONF_DEST_DIR}/qt.conf\" \"\")
#    " COMPONENT Runtime)

##--------------------------------------------------------------------------------
## Installation stuff below
##--------------------------------------------------------------------------------

##--------------------------------------------------------------------------------
## Use BundleUtilities to get all other dependencies for the application to work.
## It takes a bundle or executable along with possible plugins and inspects it
## for dependencies.  If they are not system dependencies, they are copied.

## directories to look for dependencies
#SET(DIRS ${QT_LIBRARY_DIRS} ${OPENSCENEGRAPH_INCLUDE_DIRS} libs/FRE_Slicer)

## Now the work of copying dependencies into the bundle/package
## The quotes are escaped and variables to use at install time have their $ escaped
## An alternative is the do a configure_file() on a script and use install(SCRIPT  ...).
#INSTALL(CODE "
#    file(GLOB_RECURSE QTPLUGINS
#      \"\${CMAKE_INSTALL_PREFIX}/${PLUGIN_DEST_DIR}/plugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
#    include(BundleUtilities)
#    fixup_bundle(\"${APPS}\" \"\${QTPLUGINS}\" \"${DIRS}\")
#    " COMPONENT Runtime)

##--------------------------------------------------------------------------------
## To Create a package, one can run "cpack -G DragNDrop CPackConfig.cmake" on Mac OS X
## where CPackConfig.cmake is created by including CPack
## And then there's ways to customize this as well
#set(CPACK_BINARY_DRAGNDROP ON)

#--------------------------------------------------------------------------------
# Set the CPACK Generator.  This depends on what generator you want to use
# and which platform you are on.  NSIS works on windows.
#SET(CPACK_GENERATOR "NSIS") # this needs NSIS installed, and available


#SET(CPACK_PACKAGE_NAME                      "FRE_INSTALL")
#SET(CPACK_PACKAGE_VENDOR                    "Taylor Greenwood")
##SET(CPACK_PACKAGE_DIRECTORY                 "C:/MultiMaterial")
#SET(CPACK_PACKAGE_VERSION                   "1.0.0")
#SET(CPACK_PACKAGE_VERSION_MAJOR             "1")
#SET(CPACK_PACKAGE_VERSION_MINOR             "0")
#SET(CPACK_PACKAGE_VERSION_PATCH             "0")
#SET(CPACK_PACKAGE_DESCRIPTION_FILE)
#SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY       "FRE Visualizer")
#SET(CPACK_PACKAGE_HOMEPAGE_URL)
#SET(CPACK_PACKAGE_FILE_NAME                 "fre_visualizer_install")
#SET(CPACK_PACKAGE_INSTALL_DIRECTORY         "FRE Install")
#SET(CPACK_PACKAGE_ICON)


#set(CPACK_COMPONENT_RUNTIME_DISPLAY_NAME    "FRE Visualizer Application")
#set(CPACK_COMPONENT_RUNTIME_DESCRIPTION     "Runtime description")

##--------------------------------------------------------------------------------
## This tells CPack how to add the application to the start menu in windows or
## equivalant on other platforms.
#SET(CPACK_PACKAGE_EXECUTABLES "${PROJECT_NAME};FREVisualizer")
#SET(CPACK_PACKAGE_DIRECTORY "C:/MultiMaterial")

#IF(WIN32 AND NOT UNIX)
#  # There is a bug in NSI that does not handle full unix paths properly. Make
#  # sure there is at least one set of four (4) backlasshes.
#  #SET(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
#  #SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\${PROJECT_NAME}.exe")
#  SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY}")
#  #SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.my-project-home-page.org")
#  #SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.my-personal-home-page.com")
#  SET(CPACK_NSIS_CONTACT "taylorgwood@gmail.com")
#ELSE(WIN32 AND NOT UNIX)
#  SET(CPACK_STRIP_FILES "bin/${PROJECT_NAME}")
#  SET(CPACK_SOURCE_STRIP_FILES "")
#ENDIF(WIN32 AND NOT UNIX)


#--------------------------------------------------------------------------------
# Tell CPACK not to include the contents of the .git folder
#SET(CPACK_SOURCE_IGNORE_FILES "${CMAKE_SOURCE_DIR}/.git/")

#--------------------------------------------------------------------------------
# This must be included last.  This does all the work.
#INCLUDE(CPack)
