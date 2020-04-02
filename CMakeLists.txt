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

if(UNIX AND NOT APPLE AND NOT ANDROID)
   set(LINUX TRUE)
endif()

if(MACOSX)
   FILE(GLOB_RECURSE MAC_USER_HEADER "mac/*.h")
   FILE(GLOB_RECURSE MAC_USER_CPP "mac/*.cpp")
   list(
      APPEND APP_SOURCE
      ${MAC_USER_CPP}
   )
   list(
      APPEND APP_HEADER
      ${MAC_USER_HEADER}
   )
endif()

if(IPHONE)
   FILE(GLOB_RECURSE IPHONE_USER_HEADER "ios/*.h")
   FILE(GLOB_RECURSE IPHONE_USER_CPP "ios/*.cpp")
   list(
      APPEND APP_SOURCE
      ${IPHONE_USER_CPP}
   )
   list(
      APPEND APP_HEADER
      ${IPHONE_USER_HEADER}
   )
endif()

if(WIN32)
   FILE(GLOB_RECURSE WINDOWS_USER_HEADER "windows/*.h")
   FILE(GLOB_RECURSE WINDOWS_USER_CPP "windows/*.cpp")
   list(
      APPEND APP_SOURCE
      ${WINDOWS_USER_CPP}
   )
   list(
      APPEND APP_HEADER
      ${WINDOWS_USER_HEADER}
   )
endif()

if(LINUX)
   FILE(GLOB_RECURSE LINUX_USER_HEADER "linux/*.h")
   FILE(GLOB_RECURSE LINUX_USER_CPP "linux/*.cpp")
   list(
      APPEND APP_SOURCE
      ${LINUX_USER_CPP}
   )
   list(
      APPEND APP_HEADER
      ${LINUX_USER_HEADER}
   )
endif()

if(ANDROID)
   FILE(GLOB_RECURSE ANDROID_USER_HEADER "android/cpp/*.h")
   FILE(GLOB_RECURSE ANDROID_USER_CPP "android/cpp/*.cpp")
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
else()
   if(MACOSX)
      include_directories( ${OPENGL_INCLUDE_DIRS} )
      include_directories( ${GLUT_INCLUDE_DIRS} )
      find_package(OpenGL REQUIRED)
      find_package(GLUT REQUIRED)
      target_link_libraries( ${PROJECT_NAME} ${GLUT_LIBRARY} ${OPENGL_LIBRARIES})
   elseif(WIN32)
      find_package(OpenGL REQUIRED)
      find_package(GLUT REQUIRED)
      include_directories(${OPENGL_INCLUDE_DIRS})
      include_directories( ${GLUT_INCLUDE_DIRS} )
      target_link_libraries(${PROJECT_NAME} GLUT::GLUT)
      target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
   elseif(LINUX)
      find_package(OpenGL REQUIRED)
      find_package(GLUT REQUIRED)
      find_package(X11 REQUIRED)
      include_directories(${OPENGL_INCLUDE_DIRS})
      include_directories( ${GLUT_INCLUDE_DIRS} )
      include_directories(${X11_INCLUDE_DIRS})
      target_link_libraries(${PROJECT_NAME} X11)
      target_link_libraries(${PROJECT_NAME} GLUT::GLUT)
      target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
   endif()
   set(CMAKE_BINARY_DIR ".")
   set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
   set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
#  file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
endif()


