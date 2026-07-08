SET(SRC
    World.cpp
    WorldLoader.cpp
    entities/Bomb.cpp
    entities/Character.cpp
    entities/Grass.cpp
    entities/PowerUp.cpp
    entities/Wall.cpp
    Stopwatch.cpp
    Random.cpp
    Subject.cpp
)

ADD_LIBRARY(core STATIC ${SRC})

target_include_directories(core PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)
