


cmake_minimum_required(VERSION 3.16)
project(GradeWordApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia Network Sql)

qt_standard_project_setup()

set(SOURCES
    main.cpp
    basetab.cpp
    basetab.h
    mainwindow.cpp
    mainwindow.h


    wordbooktab.cpp
    wordbooktab.h
    reviewtab.cpp
    reviewtab.h
    dictationtab.cpp
    dictationtab.h



    downloader.cpp
)

qt_add_executable(GradeWordApp ${SOURCES}
    audioplayer.cpp
    audioplayer.h
    wordmanager.cpp   # ← 加这一行
    wordmanager.h
)

set_target_properties(GradeWordApp PROPERTIES
    WIN32_EXECUTABLE TRUE)
target_link_libraries(GradeWordApp PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::Network
    Qt6::Sql
)
