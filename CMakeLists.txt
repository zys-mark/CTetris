cmake_minimum_required (VERSION 2.6)

project (CTetris)

file(GLOB tetris "src/*.cpp")
# add fontcache library directory
include_directories(${PROJECT_SOURCE_DIR}/dep_libs)
link_directories(${PROJECT_SOURCE_DIR}/dep_libs)

add_executable(CTetris ${tetris})
target_link_libraries(CTetris fontcache)
TARGET_LINK_LIBRARIES(CTetris SDL2 SDL2_ttf)