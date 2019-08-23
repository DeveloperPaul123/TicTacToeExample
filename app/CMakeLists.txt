cmake_minimum_required(VERSION 3.12.1)

project(TicTacToe-Qt5)

# set all the project source files. 
set(project_sources 
	src/main.cpp 
    src/mainwindow.cpp
    src/boardwidget.cpp)

#set the project headers. 
set(project_headers 
    include/mainwindow.h
    include/boardwidget.h)
    
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
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${PROJECT_NAME}
	PUBLIC
		Qt5::Core 
		Qt5::Gui
        Qt5::Widgets
        tictactoe
)

# installer rules. 
install(TARGETS ${PROJECT_NAME}
    DESTINATION bin COMPONENT binaries)
    
#custom commands for copying dll to build directory so .exe works. 
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Qt5::Core> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Qt5::Widgets> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Qt5::Gui> $<TARGET_FILE_DIR:${PROJECT_NAME}>
)