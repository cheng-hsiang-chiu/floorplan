# CMake version
cmake_minimum_required (VERSION 3.10)

# Project name
project(fplib VERSION 1.0.0 LANGUAGES CXX)

add_library(fplib floorplan.cpp floorplan.hpp)
