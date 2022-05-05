cmake_minimum_required(VERSION 3.16)
project(shadows)
find_package(PkgConfig REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_executable(shadows src/main.cpp src/common.h src/Shader.cpp src/Shader.h src/Object.cpp src/Object.h src/Camera.cpp src/Camera.h src/Light.cpp src/Light.h)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)
include_directories(shadows GLM_INCLUDE_DIR)

find_package(assimp REQUIRED)
include_directories(shadows ASSIMP_INCLUDE_DIRS)

pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

target_link_libraries(shadows OpenGL::OpenGL ${GLFW_LIBRARIES} GLEW::GLEW ${ASSIMP_LIBRARIES})
