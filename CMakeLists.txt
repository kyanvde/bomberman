CMAKE_MINIMUM_REQUIRED(VERSION 3.28.3)

PROJECT(Bomberman)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Disable compiler-specific extensions (like GNU extensions) to ensure portability
set(CMAKE_CXX_EXTENSIONS OFF)

ADD_SUBDIRECTORY(game)
ADD_SUBDIRECTORY(core)

enable_testing()
ADD_SUBDIRECTORY(tests)

# Documentation
find_package(Doxygen)

if(Doxygen_FOUND)
    set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/docs")

    set(DOXYGEN_PROJECT_NAME "Bomberman")
    set(DOXYGEN_PROJECT_VERSION "1.0")
    set(DOXYGEN_PROJECT_DESCRIPTION "Bomberman game")

    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_LATEX NO)

    doxygen_add_docs(
            documentation
            ${PROJECT_SOURCE_DIR}/game
            ${PROJECT_SOURCE_DIR}/core
            COMMENT "Generating API documentation"
    )
else()
    message(WARNING "Doxygen not found - documentation target unavailable")
endif()
