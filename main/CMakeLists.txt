
## --------------------------------------------------------
## main program 1: floorplan
## --------------------------------------------------------
add_executable(floorplan main.cpp)
target_link_libraries(floorplan libfloorplan error_settings)
target_include_directories(floorplan PRIVATE ${FP_SRC_DIR})
