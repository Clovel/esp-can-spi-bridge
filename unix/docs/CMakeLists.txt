# 
#                     Copyright (C) 2020 Clovis Durand
# 
# -----------------------------------------------------------------------------

# CMakeLists.txt that generates the Doxygen documentation
find_package(Doxygen)
option(BUILD_DOC "Build API documentation" ${DOXYGEN_FOUND})

set(doxy_main_page ${CMAKE_CURRENT_SOURCE_DIR}/../../README.md)

if(DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN  ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target(${CMAKE_PROJECT_NAME}-doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
else(DOXYGEN_FOUND)
    message("[WARN ] Doxygen is required to generate the doxygen documentation")
endif(DOXYGEN_FOUND)

set(HTML_DOCS_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/html
)

set(LATEX_DOCS_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/latex
)

set(MAN_DOCS_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/man
)
