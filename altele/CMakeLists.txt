cmake_minimum_required(VERSION 3.16)
project(auction_house_sfml LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML COMPONENTS Graphics Window System CONFIG REQUIRED)

add_executable(auction_sfml "main_sfml.cpp")

target_link_libraries(auction_sfml PRIVATE SFML::Graphics SFML::Window SFML::System)
