add_subdirectory(SDL3)

add_library(vendors INTERFACE)
target_link_libraries(vendors INTERFACE SDL3::SDL3)