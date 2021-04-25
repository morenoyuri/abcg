project(2d_game)
add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp bats.cpp drone.cpp)
enable_abcg(${PROJECT_NAME})