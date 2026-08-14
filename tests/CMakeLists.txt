SET(TEST_SRC
    main.cpp
    Vector2Tests.cpp
    CameraTests.cpp
    RandomTests.cpp
    StopwatchTests.cpp
)

ADD_EXECUTABLE(core_tests ${TEST_SRC})

target_link_libraries(core_tests PRIVATE core)

find_package(Threads REQUIRED)
target_link_libraries(core_tests PRIVATE Threads::Threads)

target_compile_options(core_tests PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

add_test(NAME core_tests COMMAND core_tests)
