cmake_minimum_required(VERSION 3.28)
project(DCHP_Project)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        DCHP_Project main.cpp
        dhcp_offer.h
        dhcp_offer.cpp
        config.h
        config.cpp
)
