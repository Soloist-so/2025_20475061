cmake_minimum_required(VERSION 3.15)
project(CalcProject LANGUAGES CXX)

# IMPORTANT: set install prefix early + in cache (VS/CMake caches this)
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Install path" FORCE)

# Targets
add_library(maths adder.cpp)
add_executable(calc calc.cpp)
target_link_libraries(calc PRIVATE maths)

# Install rules
install(TARGETS calc maths
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
)

# Package generator selection
if(WIN32)
    set(CPACK_GENERATOR "NSIS")
else()
    set(CPACK_GENERATOR "ZIP")
endif()

include(CPack)

