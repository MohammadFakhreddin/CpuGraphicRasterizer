cmake_minimum_required(VERSION 3.4.1)

set(PROJECT_NAME 3d-cube)

project(3d-cube)

SET(CMAKE_CXX_STANDARD 14)
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

if(APPLE)
   if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
      set(MACOSX TRUE)
   elseif()
      set(IPHONE TRUE)
   endif()
endif()

if(MACOSX)
   FILE(GLOB_RECURSE MAC_USER_HEADER "mac/*.h")
   FILE(GLOB_RECURSE MAC_USER_CPP "mac/*.cpp")
   # for MacOS X or iOS, watchOS, tvOS (since 3.10.3)
   list(
      APPEND APP_SOURCE
      ${MAC_USER_CPP}
   )
   list(
      APPEND APP_HEADER
      ${MAC_USER_HEADER}
   )
endif()

if(ANDROID)
   FILE(GLOB_RECURSE ANDROID_USER_HEADER "android/app/src/main/java/co/fakhreddin/cube/*.h")
   FILE(GLOB_RECURSE ANDROID_USER_CPP "android/app/src/main/java/co/fakhreddin/cube/*.cpp")
   list(
      APPEND APP_SOURCE
      ${ANDROID_USER_CPP}
   )
   list(
      APPEND APP_HEADER
      ${ANDROID_USER_HEADER}
   )
   add_library(${PROJECT_NAME} SHARED  ${APP_HEADER} ${APP_SOURCE})
else()
   add_executable(${PROJECT_NAME} ${APP_HEADER} ${APP_SOURCE})
endif()

if(ANDROID)
   find_library(EGL REQUIRED)
   find_library(GLESv2 REQUIRED)
   target_link_libraries( ${PROJECT_NAME} EGL GLESv2)
elseif(MACOSX)
   # # add extra include directories
   include_directories(/usr/local/include)
   # # add extra lib directories
   link_directories(/usr/local/lib)
   include_directories( ${OPENGL_INCLUDE_DIRS} )
   include_directories( ${GLUT_INCLUDE_DIRS} )
   find_package(OpenGL REQUIRED)
   find_package(GLUT REQUIRED)
   target_link_libraries( ${PROJECT_NAME} ${GLUT_LIBRARY} ${OPENGL_LIBRARIES})
endif()


