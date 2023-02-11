cmake_minimum_required(VERSION 3.14.7)

project(nodes2c)

set(NODES2C_APP_NAME "nodes2c" CACHE STRING "The name of the application.")
set(NODES2C_APP_ID   "nodes2c" CACHE STRING "The name of the application as a C identifier.")
set(NODES2C_APP_EXT  "nodes"   CACHE STRING "The file extension for project files.")

include(FetchContent)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_Declare(glfw URL "https://github.com/glfw/glfw/releases/download/3.3.8/glfw-3.3.8.zip")
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(imgui URL "https://github.com/ocornut/imgui/archive/refs/tags/v1.89.2.zip")
FetchContent_MakeAvailable(imgui)
FetchContent_GetProperties(imgui)

FetchContent_Declare(imnodes URL "https://github.com/Nelarius/imnodes/archive/d88f99125bb72cdb71b4c27ff6eb7f318d89a4c5.zip")
FetchContent_GetProperties(imnodes)
if(NOT ${imnodes_POPULATED})
  FetchContent_Populate(imnodes)
endif(NOT ${imnodes_POPULATED})

#======#
# glad #
#======#

add_library(nodes2c_glad
  glad/include/glad/glad.h
  glad/include/GLES2/gl2.h
  glad/include/KHR/khrplatform.h
  glad/src/glad.c)

target_include_directories(nodes2c_glad PUBLIC glad/include)

target_compile_definitions(nodes2c_glad PUBLIC GLFW_INCLUDE_NONE=1)

#=======#
# imgui #
#=======#

add_library(nodes2c_imgui STATIC
  "${imgui_SOURCE_DIR}/imconfig.h"
  "${imgui_SOURCE_DIR}/imgui.h"
  "${imgui_SOURCE_DIR}/imgui_internal.h"
  "${imgui_SOURCE_DIR}/imstb_rectpack.h"
  "${imgui_SOURCE_DIR}/imstb_textedit.h"
  "${imgui_SOURCE_DIR}/imstb_truetype.h"
  "${imgui_SOURCE_DIR}/imgui.cpp"
  "${imgui_SOURCE_DIR}/imgui_draw.cpp"
  "${imgui_SOURCE_DIR}/imgui_tables.cpp"
  "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp")

target_compile_definitions(nodes2c_imgui PRIVATE IMGUI_IMPL_OPENGL_ES2=1)

target_include_directories(nodes2c_imgui
  PUBLIC
    "${imgui_SOURCE_DIR}"
    "${imgui_SOURCE_DIR}/backends")

target_link_libraries(nodes2c_imgui PUBLIC nodes2c_glad glfw)

#=========#
# imnodes #
#=========#

add_library(nodes2c_imnodes STATIC
  "${imnodes_SOURCE_DIR}/imnodes.h"
  "${imnodes_SOURCE_DIR}/imnodes_internal.h"
  "${imnodes_SOURCE_DIR}/imnodes.cpp")

target_include_directories(nodes2c_imnodes PUBLIC ${imnodes_SOURCE_DIR})

target_link_libraries(nodes2c_imnodes PUBLIC nodes2c_imgui)

#=========#
# nodes2c #
#=========#

add_executable(nodes2c WIN32
  app.hpp
  app.cpp
  model.hpp
  model.cpp
  main.cpp)

target_link_libraries(nodes2c PRIVATE glfw nodes2c_glad nodes2c_imgui nodes2c_imnodes)

target_include_directories(nodes2c PRIVATE glad/include)
