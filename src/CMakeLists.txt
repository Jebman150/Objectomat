cmake_minimum_required(VERSION "3.20.0")
project(objectomat)
add_executable(${PROJECT_NAME} 
                main.cpp
                mathFunc.cpp
                fileReader.cpp
                bodys.cpp
                mesh.cpp
                meshVertex.cpp
                )

target_include_directories(${PROJECT_NAME} PUBLIC
    ../libraries/SFML-2.5.1/include)

target_link_libraries(${PROJECT_NAME}
    sfml-window
    sfml-graphics
    sfml-system
    )