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
        Dealer.cpp
        Dealer.h
        Deck.cpp
        Deck.h
        GameLogic.cpp
        GameLogic.h
        GameGraphics.cpp
        GameGraphics.h

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
# GOOGLE TEST

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(GameLogicTests
        Tests/GameLogicTest.cpp
        GameLogic.cpp
        Tests/DeckTest.cpp
        Tests/DealerTest.cpp
        Tests/PlayerTest.cpp

        # FILE DEL PROGETTO CHE DEVONO ESSERE LINKATI
        GameLogic.cpp
        Deck.cpp
        Dealer.cpp
        Player.cpp
        Carta.cpp
)

target_include_directories(GameLogicTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(GameLogicTests
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(GameLogicTests)

file(GLOB_RECURSE ASSET_FILES
        ${CMAKE_SOURCE_DIR}/assets/*
)

add_custom_target(copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
)

add_dependencies(Blackjack1 copy_assets)
