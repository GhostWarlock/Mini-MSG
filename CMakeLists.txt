#cmake_minimum_required(VERSION 3.17)
#project(Mini_MSG)
#
#set(CMAKE_CXX_STANDARD 11)
#
#add_executable(Mini_MSG main.cpp)



cmake_minimum_required(VERSION 3.17)
project(Mini_MSG LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Threads)

set(include_path ./view/)
set(src_files main.cpp
        view/loginwindow.cpp
        view/loginwindow.h
        loginwindow.qrc
        view/resources.h view/tools.cpp view/tools.h)

include_directories(${include_path})
add_executable(Mini_MSG ${src_files})

target_link_libraries(Mini_MSG PRIVATE ${CMAKE_THREAD_LIBS_INIT}
        Qt${QT_VERSION_MAJOR}::Widgets)
