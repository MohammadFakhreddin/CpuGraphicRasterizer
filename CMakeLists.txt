cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME 3d-cube)

project(3d-cube)

SET(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

FILE(GLOB_RECURSE USER_HEADER "src/*.h")
FILE(GLOB_RECURSE USER_CPP "src/*.cpp")

set(APP_SOURCE)
set(APP_HEADER)

list(
   APPEND APP_SOURCE
   ${USER_CPP}
)

list(
    APPEND APP_HEADER
   ${USER_HEADER}
)

add_executable(${PROJECT_NAME} ${APP_HEADER} ${APP_SOURCE})

include_directories( ${OPENGL_INCLUDE_DIRS} )
include_directories( ${GLUT_INCLUDE_DIRS} )

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

target_link_libraries( ${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

