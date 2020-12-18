add_library(libfloorplan floorplan.cpp floorplan.hpp)
add_library(libfloorplantester floorplan_tester.cpp floorplan_tester.hpp)

target_link_libraries(libfloorplan error_settings)
target_link_libraries(libfloorplantester libfloorplan error_settings)


