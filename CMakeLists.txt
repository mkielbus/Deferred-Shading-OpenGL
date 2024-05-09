cmake_minimum_required(VERSION 3.15)

project(gkom24l VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(gkom24l main.cpp SingletonApp.cpp ShadersConf.cpp Data.hpp)

target_include_directories(gkom24l PRIVATE ${INCLUDE_DIR})

find_library(GLEW_LIB GLEW ${LIB_DIR} REQUIRED)
find_library(GLFW3_LIB glfw ${LIB_DIR} REQUIRED)
find_library(GL_LIB GL ${LIB_DIR} REQUIRED)

target_link_libraries(gkom24l PRIVATE ${GLEW_LIB} ${GLFW3_LIB} ${GL_LIB})