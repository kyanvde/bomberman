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
# The dot component (Graphviz) is optional: with it, Doxygen renders the class-structure and
# inheritance diagrams; without it, documentation still generates, just as text-only listings.
find_package(Doxygen OPTIONAL_COMPONENTS dot)

if(Doxygen_FOUND)
    set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/docs")

    set(DOXYGEN_PROJECT_NAME "Bomberman")
    set(DOXYGEN_PROJECT_VERSION "1.0")
    set(DOXYGEN_PROJECT_DESCRIPTION "Bomberman game")

    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_LATEX NO)

    # UML-style class diagrams, plus a graphical inheritance hierarchy for the whole project.
    # UML_LIMIT_NUM_FIELDS keeps the boxes readable: EntityModel alone has ~40 members, which
    # would otherwise render as one unreadably tall box.
    set(DOXYGEN_UML_LOOK YES)
    set(DOXYGEN_UML_LIMIT_NUM_FIELDS 10)
    set(DOXYGEN_CLASS_GRAPH YES)
    set(DOXYGEN_COLLABORATION_GRAPH YES)
    set(DOXYGEN_GRAPHICAL_HIERARCHY YES)
    set(DOXYGEN_DOT_IMAGE_FORMAT svg)

    doxygen_add_docs(
            documentation
            ${PROJECT_SOURCE_DIR}/game
            ${PROJECT_SOURCE_DIR}/core
            COMMENT "Generating API documentation"
    )
else()
    message(WARNING "Doxygen not found - documentation target unavailable")
endif()
