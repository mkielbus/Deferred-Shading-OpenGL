cmake_minimum_required(VERSION 3.15)

project(gkom24l VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(gkom24l main.cpp SingletonApp.cpp ShadersConf.cpp Data.hpp Framebuffer.cpp)

target_include_directories(gkom24l PRIVATE ${INCLUDE_DIR})

if(WIN32)
    # Konfiguracja dla Windowsa
    find_library(GLEW_LIB NAMES glew32 PATHS ${LIB_DIR} REQUIRED)
    find_library(GLFW3_LIB NAMES glfw3 PATHS ${LIB_DIR} REQUIRED)
    find_library(GLEWS_LIB NAMES glew32s PATHS ${LIB_DIR} REQUIRED)
find_library(GL_LIB NAMES opengl32 PATHS ${LIB_DIR} REQUIRED)

    target_link_libraries(gkom24l PRIVATE ${GLEW_LIB} ${GLFW3_LIB} ${GLEWS_LIB} ${GL_LIB})
elseif(UNIX)
    # Konfiguracja dla Linuxa
    find_library(GLEW_LIB GLEW REQUIRED)
    find_library(GLFW3_LIB glfw REQUIRED)
    find_library(GL_LIB GL REQUIRED)

    target_link_libraries(gkom24l PRIVATE ${GLEW_LIB} ${GLFW3_LIB} ${GL_LIB})
endif()