CMAKE_MINIMUM_REQUIRED(VERSION 3.28.3)

PROJECT(Bomberman)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Disable compiler-specific extensions (like GNU extensions) to ensure portability
set(CMAKE_CXX_EXTENSIONS OFF)

SET(CMAKE_CXX_FLAGS "-Wall -Werror")

ADD_SUBDIRECTORY(game)
ADD_SUBDIRECTORY(core)
