cmake_minimum_required(VERSION 3.31)
project(kalendarz)

set(CMAKE_CXX_STANDARD 20)

add_executable(kalendarz
        CalendarManager.cpp
        Item.cpp
        TimeUtils.cpp)
