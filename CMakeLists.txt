#### TicTacToe #####
# Copyright Paul Tsouchlos
# Author: Paul Tsouchlos
# Last Update: November 2, 2015

cmake_minimum_required(VERSION 3.3)

# Set the version number
set (TIC_TAC_TOE_VERSION_MAJOR "0")
set (TIC_TAC_TOE_VERSION_MINOR "1")
set (TIC_TAC_TOE_VERSION_PATCH "0")
set (TIC_TAC_TOE_VERSION "${TIC_TAC_TOE_VERSION_MAJOR}.${TIC_TAC_TOE_VERSION_MINOR}.${TIC_TAC_TOE_VERSION_PATCH}")

#set project name. 
project(TicTacToe)

set(CMAKE_PREFIX_PATH "C:/Qt/5.5/msvc2013_64/lib/cmake")
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)

#set all the project source files. 
set(Project_Sources 
	main.cpp 
	mainwindow.cpp
	TicTacToe.cpp)

#set the project headers. 
set(Project_Headers 
	mainwindow.h
	TicTacToe.h)

#set project forms
set(Project_Forms mainwindow.ui)

#find needed project dependencies. 
find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)

#wrap the sources and the ui. 
QT5_WRAP_CPP(Project_HEADERS_MOC ${Project_Headers})
QT5_WRAP_UI(Project_FORMS_HEADERS ${Project_Forms})

#add second project to wrap the gui. Updates to the ui files will show up 
#only after you rebuild. 
add_library(GUI ${Project_FORMS_HEADERS} ${Project_FORMS_HEADERS})
target_link_libraries(GUI Qt5::Widgets Qt5::Core Qt5::Gui)

#add other sources, libs and headers as executables. 
add_executable(TicTacToe WIN32 ${Project_Sources} ${GUI} ${Project_Headers})
target_link_libraries(TicTacToe GUI)

set(QT_VERSION_MAJOR} "5")

#set properties. 
if(WIN32)
	set(QT_USE_QTMAIN true)
	
	
	if(MSVC)
		set_target_properties(TicTacToe PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
		
		#installer rules. 
		install(TARGETS TicTacToe
			DESTINATION bin COMPONENT bin_tictactoe)
			
		 install(FILES
        ${Qt5Core_DIR}/../../../bin/libEGL.dll
        ${Qt5Core_DIR}/../../../bin/libGLESv2.dll
        ${Qt5Core_DIR}/../../../bin/Qt5Core.dll
        ${Qt5Core_DIR}/../../../bin/Qt5Gui.dll
        ${Qt5Core_DIR}/../../../bin/Qt5Declarative.dll
        ${Qt5Core_DIR}/../../../bin/Qt5Network.dll
        ${Qt5Core_DIR}/../../../bin/Qt5OpenGL.dll
		${Qt5Core_DIR}/../../../bin/Qt5Script.dll
		${Qt5Core_DIR}/../../../bin/Qt5Sql.dll
		${Qt5Core_DIR}/../../../bin/Qt5Widgets.dll
		${Qt5Core_DIR}/../../../bin/Qt5Xml.dll
		${Qt5Core_DIR}/../../../bin/Qt5XmlPatterns.dll
        DESTINATION bin COMPONENT bin_tictactoe
    )
	
	install(FILES	
		${Qt5Core_DIR}/../../../plugins/imageformats/qdds.dll
		${Qt5Core_DIR}/../../../plugins/imageformats/qgif.dll
		${Qt5Core_DIR}/../../../plugins/imageformats/qicns.dll
		${Qt5Core_DIR}/../../../plugins/imageformats/qico.dll
		${Qt5Core_DIR}/../../../plugins/imageformats/qjp2.dll
		${Qt5Core_DIR}/../../../plugins/imageformats/qjpeg.dll
		${Qt5Core_DIR}/../../../plugins/imageformats/qmng.dll
		${Qt5Core_DIR}/../../../plugins/imageformats/qsvg.dll
		${Qt5Core_DIR}/../../../plugins/imageformats/qtga.dll
		${Qt5Core_DIR}/../../../plugins/imageformats/qtiff.dll
		${Qt5Core_DIR}/../../../plugins/imageformats/qwbmp.dll
		${Qt5Core_DIR}/../../../plugins/imageformats/qwebp.dll
		DESTINATION bin/imageformats COMPONENT bin_tictactoe)
		
		install(FILES
		${Qt5Core_DIR}/../../../plugins/platforms/qwindows.dll
		DESTINATION bin/platforms COMPONENT bin_tictactoe)
	else()
		message(SEND_ERROR "Unsupported Compiler")
	endif()	

	#installer stuff
	SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "TicTacToe")
	SET(CPACK_PACKAGE_VENDOR "Paul Tsouchlos")
	SET(CPACK_PACKAGE_VERSION_MAJOR ${TIC_TAC_TOE_VERSION_MAJOR})
	SET(CPACK_PACKAGE_VERSION_MINOR ${TIC_TAC_TOE_VERSION_MINOR})
	SET(CPACK_PACKAGE_VERSION_PATCH ${TIC_TAC_TOE_VERSION_PATCH})
	SET(CPACK_PACKAGE_INSTALL_DIRECTORY "TicTacToe ${TIC_TAC_TOE_VERSION_MAJOR}.${TIC_TAC_TOE_VERSION_MINOR}")
	SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\TicTacToe.exe")
	SET(CPACK_NSIS_DISPLAY_NAME "TicTacToe - ${TIC_TAC_TOE_VERSION}")
	SET(CPACK_NSIS_MODIFY_PATH ON)
	SET(CPACK_COMPONENTS_ALL bin_tictactoe)
	if (CMAKE_CL_64)
		set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
	else (CMAKE_CL_64)
		set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
	endif (CMAKE_CL_64)
elseif(UNIX)
	SET(CPACK_STRIP_FILES "bin/TicTacToe")
	SET(CPACK_SOURCE_STRIP_FILES "")
else()
	message(SEND_ERROR "Unsupported platform")
endif()

include(CPACK)

#custom commands for copying dll to build directory so .exe works. 
add_custom_command(TARGET TicTacToe POST_BUILD 
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/libEGLd.dll          ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/libGLESv2d.dll       ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Cored.dll         ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Guid.dll          ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Declaratived.dll  ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Networkd.dll      ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5OpenGLd.dll       ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Scriptd.dll       ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Sqld.dll          ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Widgetsd.dll      ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Xmld.dll          ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5XmlPatternsd.dll  ${CMAKE_BINARY_DIR}/Debug
  
  # release version
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/libEGL.dll           ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/libGLESv2.dll        ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Core.dll          ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Gui.dll           ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Declarative.dll   ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Network.dll       ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5OpenGL.dll        ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Script.dll        ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Sql.dll           ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Widgets.dll       ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Xml.dll           ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5XmlPatterns.dll   ${CMAKE_BINARY_DIR}/Release
  
  # Output Message
  COMMENT "Copying Qt binaries from '${Qt5Core_DIR}/../../bin/' to '${CMAKE_BINARY_DIR}'" VERBATIM
)

#Add target to build documents from visual studio.
set(DOXYGEN_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
set(DOXYGEN_OUTPUT_DIR CACHE PATH "Path for document generation output")
find_package(Doxygen)

if(DOXYGEN_FOUND)

ADD_CUSTOM_TARGET(DOCUMENTATION
  COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
  COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
  COMMAND ${CMAKE_COMMAND} -E echo "Done."
  WORKING_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

endif(DOXYGEN_FOUND)
