cmake_minimum_required(VERSION "3.20.0")
project(antSim)
add_executable(${PROJECT_NAME} 
                main.cpp
                actions.cpp
                maths.cpp
                )

target_include_directories(${PROJECT_NAME} PUBLIC
    ../libraries/SFML-2.5.1/include)

target_link_libraries(${PROJECT_NAME}
    sfml-window
    sfml-graphics
    sfml-system
    sfml-audio
    )