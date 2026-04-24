cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(try_box2d)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${SRC_FILES})

# install CPM (Cmake Package Manager) for Cmake
file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.42.0/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
  # EXPECTED_HASH SHA256=cc155ce02e7945e7b8967ddfaff0b050e958a723ef7aad3766d368940cb15494
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

# install libraries with CPM
set(LIBS)

CPMAddPackage(
  NAME SDL3
  GITHUB_REPOSITORY libsdl-org/SDL
  GIT_TAG release-3.2.24
)
if(SDL3_ADDED)
  list(APPEND LIBS SDL3::SDL3)
endif()

set(SDLIMAGE_AVIF OFF CACHE BOOL "" FORCE)
CPMAddPackage(
  NAME SDL3_image
  GITHUB_REPOSITORY libsdl-org/SDL_image
  GIT_TAG release-3.2.4
)
if(SDL3_image_ADDED)
  list(APPEND LIBS SDL3_image::SDL3_image)
endif()

CPMAddPackage(
  NAME box2d
  GITHUB_REPOSITORY erincatto/box2d
  GIT_TAG v3.1.1
)
if(box2d_ADDED)
  list(APPEND LIBS box2d::box2d)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})

# Copy runtime libraries (DLL/.so) for dependencies into the executable directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:box2d::box2d> $<TARGET_FILE_DIR:${PROJECT_NAME}>
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SDL3::SDL3> $<TARGET_FILE_DIR:${PROJECT_NAME}>
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SDL3_image::SDL3_image> $<TARGET_FILE_DIR:${PROJECT_NAME}>
)