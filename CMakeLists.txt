cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(rpg-raylib C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ----------  Version ----------
find_package(Git REQUIRED)

execute_process(
  COMMAND ${GIT_EXECUTABLE} describe --tags --always --dirty
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
  ERROR_QUIET
  RESULT_VARIABLE GIT_RESULT
)

if(NOT GIT_RESULT EQUAL "0")
  set(GIT_VERSION "unknown")
endif()

# ----------  Dependencies ----------
set(SOURCES
    src/main.c
    src/utils.c
    src/list.c
    src/game.c
    src/pause.c
    src/battle.c
    src/map.c
    src/mon.c
    src/menu.c)

# Raylib graphics library
find_package(raylib 5.5 QUIET)

if(NOT raylib_FOUND)
  include(FetchContent)
  set(FETCHCONTENT_QUIET FALSE)

  set(BUILD_EXAMPLES
      OFF
      CACHE BOOL "" FORCE)
  set(BUILD_SHARED_LIBS
      OFF
      CACHE BOOL "" FORCE)
  if(WIN32)
    set(USE_MSVC_RUNTIME_LIBRARY_DLL
        ON
        CACHE BOOL "" FORCE)
  endif()

  FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
    GIT_PROGRESS TRUE)

  FetchContent_MakeAvailable(raylib)
endif()

# ---------- Primary build target ----------
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(DEBUG)
endif()

add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_definitions(${PROJECT_NAME} PRIVATE GIT_VERSION="${GIT_VERSION}")
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
target_include_directories(${PROJECT_NAME} PRIVATE include)

if(WIN32)
  set(PLATFORM_LIBS winmm opengl32 gdi32 glu32)
  target_link_libraries(${PROJECT_NAME} PRIVATE ${PLATFORM_LIBS})
elseif(APPLE)
  target_link_libraries(
    ${PROJECT_NAME} PRIVATE "-framework IOKit" "-framework Cocoa"
                            "-framework OpenGL")
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# ---------- Code quality targets ----------
if(WIN32)
  set(CHECK_SCRIPT "${CMAKE_SOURCE_DIR}/scripts/check.ps1")
  set(FORMAT_SCRIPT "${CMAKE_SOURCE_DIR}/scripts/format.ps1")
else()
  set(CHECK_SCRIPT "${CMAKE_SOURCE_DIR}/scripts/check.sh")
  set(FORMAT_SCRIPT "${CMAKE_SOURCE_DIR}/scripts/format.sh")
endif()

add_custom_target(
  check
  COMMAND ${CHECK_SCRIPT}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMENT "Running code analysis tool"
  VERBATIM)

add_custom_target(
  format
  COMMAND ${FORMAT_SCRIPT} -i
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMENT "Running clang-format on all source files"
  VERBATIM)
