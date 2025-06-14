set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

CMAKE_MINIMUM_REQUIRED(VERSION 3.30)

set(name spaceship)
project(${name})

include_directories(./output/include)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
# find_package(glm REQUIRED)

add_executable(${name} main.cpp ./output/src/glad.c renderer.cpp resource_manager.cpp shader.cpp spaceship.cpp texture.cpp stb_image.cpp)

TARGET_LINK_LIBRARIES(${name} PRIVATE OpenGL::GL)
TARGET_LINK_LIBRARIES(${name} PRIVATE glfw)
TARGET_LINK_DIRECTORIES(${name} PRIVATE /usr/include)
