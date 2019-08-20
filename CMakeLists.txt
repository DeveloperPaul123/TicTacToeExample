cmake_minimum_required(VERSION 3.12.1)

# set the version number
set (TIC_TAC_TOE_VERSION_MAJOR "0")
set (TIC_TAC_TOE_VERSION_MINOR "1")
set (TIC_TAC_TOE_VERSION_PATCH "0")
set (TIC_TAC_TOE_VERSION "${TIC_TAC_TOE_VERSION_MAJOR}.${TIC_TAC_TOE_VERSION_MINOR}.${TIC_TAC_TOE_VERSION_PATCH}")

# set project name. 
project(TicTacToe 
	LANGUAGES CXX
	VERSION ${TIC_TAC_TOE_VERSION}
	DESCRIPTION "An example implementation of TicTacToe with Qt5"
)

# set all the project source files. 
set(project_sources 
	src/main.cpp 
	src/mainwindow.cpp
	src/tictactoe.cpp)

#set the project headers. 
set(project_headers 
	include/mainwindow.h
	include/tictactoe.h)

# set project forms
set(project_forms ui/mainwindow.ui)

# find needed project dependencies. 
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

# wrap the sources and the ui. 
qt5_wrap_cpp(project_headers_moc ${project_headers})
qt5_wrap_ui(project_forms_headers ${project_forms})

# add other sources, libs and headers as executables. 
add_executable(${PROJECT_NAME} 
	${project_sources} 
	${project_headers_moc} 
	${project_headers}
	${project_forms_headers})

target_include_directories(${PROJECT_NAME} 
	PUBLIC include
	PRIVATE ${CMAKE_BINARY_DIR}
)

target_link_libraries(${PROJECT_NAME}
	PUBLIC
		Qt5::Core 
		Qt5::Gui
		Qt5::Widgets
)

# installer rules. 
install(TARGETS TicTacToe
	DESTINATION bin COMPONENT bin_tictactoe)
	
# set properties. 
if(WIN32)
	# set(QT_USE_QTMAIN true)

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
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Qt5::Core> $<TARGET_FILE_DIR:${PROJECT_NAME}>
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Qt5::Widgets> $<TARGET_FILE_DIR:${PROJECT_NAME}>
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Qt5::Gui> $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

# Add target to build documents from visual studio.
find_package(Doxygen)
if(DOXYGEN_FOUND)
	doxygen_add_docs(Documentation 
		include src 
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		COMMENT "Building documentation")
endif(DOXYGEN_FOUND)
