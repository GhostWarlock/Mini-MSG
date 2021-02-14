


cmake_minimum_required(VERSION 3.17)
project(Mini_MSG LANGUAGES CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD_REQUIRED "${CMAKE_C_FLAGS} -std=c99")

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Threads)

set(include_path ./view/)
set(src_files main.cpp

        # login UI
        view/loginwindow.cpp
        view/loginwindow.h
        loginwindow.qrc
        view/subWidget/accountBox.cpp
        view/subWidget/accountBox.h
        view/subWidget/accountItem.cpp
        view/subWidget/accountItem.h
        view/subWidget/accountBox.cpp
        view/subWidget/accountBox.h
        view/subWidget/accountItem.cpp
        view/subWidget/accountItem.h

        # Common
        view/resources.h
        tools/tools.cpp
        tools/tools.h
        sqlite3/sqlite3ext.h
        sqlite3/sqlite3.h
        sqlite3/sqlite3.c


        # view Mgt
        controller/viewMgt.cpp
        controller/viewMgt.h
        model/usrMgt.cpp model/usrMgt.h model/sqlSentence.h model/dataType.h)

include_directories(${include_path})
add_executable(Mini_MSG ${src_files})

target_link_libraries(Mini_MSG PRIVATE ${CMAKE_THREAD_LIBS_INIT}
        Qt${QT_VERSION_MAJOR}::Widgets dl)
