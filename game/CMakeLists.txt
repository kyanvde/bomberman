SET(BOMBERMAN_SOURCES main.cpp
	Game.cpp
	StateManager.cpp
    states/MenuState.cpp
    states/GameState.cpp
    states/GameOverState.cpp
    ConcreteFactory.cpp
    ConcreteRenderer.cpp
	views/CharacterView.cpp
)

ADD_EXECUTABLE(bomberman ${BOMBERMAN_SOURCES})

target_include_directories(bomberman PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/core
)

INCLUDE(FetchContent)

set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)

set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
)
FetchContent_MakeAvailable(SFML)

TARGET_LINK_LIBRARIES(bomberman PRIVATE
	core
	sfml-graphics
)

target_compile_options(bomberman PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

add_custom_command(TARGET bomberman POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		$<TARGET_FILE:sfml-graphics>
		$<TARGET_FILE_DIR:bomberman>

		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		$<TARGET_FILE:sfml-window>
		$<TARGET_FILE_DIR:bomberman>

		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		$<TARGET_FILE:sfml-system>
		$<TARGET_FILE_DIR:bomberman>

		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${PROJECT_SOURCE_DIR}/assets
		$<TARGET_FILE_DIR:bomberman>/assets
)

INSTALL(TARGETS bomberman DESTINATION bin)
IF (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    SET (CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/installed" CACHE PATH "default install path" FORCE)
ENDIF()
