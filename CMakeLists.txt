cmake_minimum_required(VERSION 3.23)

project(blaze-csv)

add_executable(blaze-csv)

target_sources(blaze-csv
    PRIVATE
        src/main.cpp
)