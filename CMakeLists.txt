cmake_minimum_required(VERSION 3.28)
project(Blackjack LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dove mettere l'eseguibile
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# FetchContent per SFML 2.6.1
include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.1
)

FetchContent_MakeAvailable(SFML)

# Lista dei sorgenti
add_executable(Blackjack1
        main.cpp
        Carta.cpp
        Carta.h
        Player.cpp
        Player.h

)

# Collega SFML (usa static linking)
target_link_libraries(Blackjack1 PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
)

# Include SFML
target_include_directories(Blackjack1 PRIVATE ${sfml_SOURCE_DIR}/include)

# Definisci SFML_STATIC per linking statico
target_compile_definitions(Blackjack1 PRIVATE SFML_STATIC)

# Librerie di sistema necessarie per Windows
if(WIN32)
    target_link_libraries(Blackjack1 PRIVATE
            opengl32
            winmm
            gdi32
            user32
            shell32
            advapi32
    )
endif()
