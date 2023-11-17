cmake_minimum_required(VERSION 3.5)

project(lab2 VERSION 0.1 LANGUAGES CXX)

set(BOOST_ROOT "C:/Users/icezg/Downloads/boost_1_83_0")

find_package(Boost REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Charts)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    solver.h
    solver.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(lab2
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(lab2 SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(lab2
            ${PROJECT_SOURCES}
        )
    endif()
endif()

include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(lab2 PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Charts
    ${Boost_LIBRARIES}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(lab2)
endif()
