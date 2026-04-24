cmake_minimum_required(VERSION 3.28)
project(Project)

set(CMAKE_CXX_STANDARD 17)

add_executable(Project main.cpp
        Game.cpp
        Game.h
        Maszyna.cpp
        Maszyna.h
        Surowiec.cpp
        Surowiec.h
        Surowiec.h
        Produkt.cpp
        Produkt.h
        Lista.cpp
        Lista.h
        ObiektBazowy.cpp
        ObiektBazowy.h
        Symulacja.cpp
        Symulacja.h)
