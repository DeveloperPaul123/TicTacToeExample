cmake_minimum_required(VERSION 3.12.1)

# set the version number
set (TIC_TAC_TOE_VERSION_MAJOR "0")
set (TIC_TAC_TOE_VERSION_MINOR "1")
set (TIC_TAC_TOE_VERSION_PATCH "0")
set (TIC_TAC_TOE_VERSION "${TIC_TAC_TOE_VERSION_MAJOR}.${TIC_TAC_TOE_VERSION_MINOR}.${TIC_TAC_TOE_VERSION_PATCH}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set project name. 
project(tictactoe-example)

add_subdirectory(tictactoe)
add_subdirectory(app)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT TicTacToe-Qt5)

# set properties. 
if(WIN32)
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
    SET(CPACK_COMPONENTS_ALL binaries)
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

# Add target to build documents from visual studio.
find_package(Doxygen)
if(DOXYGEN_FOUND)
    doxygen_add_docs(Documentation 
        include src 
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Building documentation")
endif(DOXYGEN_FOUND)