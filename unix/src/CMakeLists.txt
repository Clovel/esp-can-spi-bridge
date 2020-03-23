# 
#                     Copyright (C) 2020 Clovis Durand
# 
# -----------------------------------------------------------------------------

# Header files --------------------------------------------
file(GLOB PUBLIC_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/*.hpp
)

file(GLOB_RECURSE MODULE_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
)

set(HEADERS
    ${PUBLIC_HEADERS}
    ${MODULE_HEADERS}
)

# Source files --------------------------------------------
file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

message("MAIN SOURCES : ${SOURCES}")

# Link directories ----------------------------------------

# Target definition ---------------------------------------
add_executable(${CMAKE_PROJECT_NAME}-util
    ${SOURCES}
)
add_dependencies(${CMAKE_PROJECT_NAME}-util
    initools
)
target_link_libraries(${CMAKE_PROJECT_NAME}-util
    initools
)
set_target_properties(${CMAKE_PROJECT_NAME}-util PROPERTIES 
    PUBLIC_HEADER "${PUBLIC_HEADERS}"
)

list(REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

add_library(${CMAKE_PROJECT_NAME} SHARED
    ${SOURCES}
)
add_dependencies(${CMAKE_PROJECT_NAME}
    initools
)
target_link_libraries(${CMAKE_PROJECT_NAME}
    initools
)
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES 
    PUBLIC_HEADER "${PUBLIC_HEADERS}"
)

#----------------------------------------------------------------------------
# The installation is prepended by the CMAKE_INSTALL_PREFIX variable
install(TARGETS ${CMAKE_PROJECT_NAME}-util
RUNTIME DESTINATION bin
LIBRARY DESTINATION lib
ARCHIVE DESTINATION lib
PUBLIC_HEADER DESTINATION include
)
install(TARGETS ${CMAKE_PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)
