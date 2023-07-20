cmake_minimum_required(VERSION 3.12.1)

# basic tictactoe library
project(
    tictactoe
    LANGUAGES CXX
    VERSION ${TIC_TAC_TOE_VERSION}
    DESCRIPTION "TicTacToe game library."
)

# set all the project source files.
set(project_sources # src/tictactoe.cpp
    src/board.cpp src/computer_player.cpp
)

# set the project headers.
set(__prefix include/tictactoe)
set(project_headers
    # ${__prefix}/tictactoe.h
    ${__prefix}/detail/point.h
    ${__prefix}/detail/player.h
    ${__prefix}/detail/board.h
    ${__prefix}/detail/computer_player.h
    ${__prefix}/tictactoe
)

add_library(${PROJECT_NAME} STATIC ${project_headers} ${project_sources})

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

# setup tests
file(GLOB_RECURSE test_sources tests/*.cpp)
add_executable(${PROJECT_NAME}_test ${test_sources})
target_link_libraries(
    ${PROJECT_NAME}_test
    PRIVATE 
        ${PROJECT_NAME}
        snitch::snitch
)
