


cmake_minimum_required(VERSION 3.16)
project(GradeWordApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia Network Sql)

qt_standard_project_setup()

set(SOURCES
    main.cpp
        mainwindow.cpp mainwindow.h mainwindow.ui
        wordmanager.cpp wordmanager.h
        wordbooktab.cpp wordbooktab.h wordbooktab.ui
        reviewtab.cpp reviewtab.h reviewtab.ui
        dictationtab.cpp dictationtab.h dictationtab.ui
        audiofetcher.cpp audiofetcher.h
        audiomanager.cpp
        downloader.cpp
        wordbooktab.cpp
        wordbooktab.h
)

qt_add_executable(GradeWordApp ${SOURCES}
    mainwindow.ui
    basetab.h
    basetab.cpp)

target_link_libraries(GradeWordApp PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::Network
    Qt6::Sql
)