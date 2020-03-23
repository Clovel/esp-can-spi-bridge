# 
#                     Copyright (C) 2020 Clovis Durand
# 
# -----------------------------------------------------------------------------

# Definitions ---------------------------------------------
add_definitions(-DTEST)

# Requirements --------------------------------------------

# Header files --------------------------------------------
file(GLOB_RECURSE PUBLIC_HEADERS 
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/*.hpp
)
set(HEADERS
    ${PUBLIC_HEADERS}
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc
)

# Source files --------------------------------------------
file(GLOB_RECURSE TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

# Target definition ---------------------------------------
add_executable(${CMAKE_PROJECT_NAME}-tests
    ${TEST_SOURCES}
)

# Test definition -----------------------------------------
#add_test( testname Exename arg1 arg2 ... )
add_test( osco_test_default ${CMAKE_PROJECT_NAME}-tests -1 )
