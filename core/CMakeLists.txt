SET(SRC
    World.cpp
)

ADD_LIBRARY(core STATIC ${SRC})

target_include_directories(core PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)
