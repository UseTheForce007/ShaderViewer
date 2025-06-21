cmake_minimum_required(VERSION 3.10)
project(ShaderViewer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optionally force MSYS2/MinGW-w64 if detected
if(DEFINED ENV{MSYSTEM} AND (("$ENV{MSYSTEM}" STREQUAL "MINGW64") OR ("$ENV{MSYSTEM}" STREQUAL "MINGW32")))
    message(STATUS "Detected MSYS2 environment: $ENV{MSYSTEM}")
    set(CMAKE_C_COMPILER "gcc")
    set(CMAKE_CXX_COMPILER "g++")
    set(CMAKE_MAKE_PROGRAM "mingw32-make" CACHE FILEPATH "MSYS2 make program" FORCE)
endif()

# Find packages
find_package(OpenGL REQUIRED)

# Add external dependencies
# add_subdirectory(external)

# Add source files
add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/Shader.cpp
    src/ObjModel.cpp  # Using ObjModel for OBJ loading
    src/Camera.cpp
    # src/Model.cpp  # Commented out, not used for OBJ test
)

add_library(glad STATIC external/glad/gl.c)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    C:/msys64/mingw64/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external
    ${CMAKE_CURRENT_SOURCE_DIR}/external/tinyobjloader
    C:/msys64/mingw64/include/glm
)

target_include_directories(glad PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/external
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw3
    glad
    OpenGL::GL
)

# Add ImGui library from ../imgui
add_library(imgui
    ../imgui/imgui.cpp
    ../imgui/imgui_draw.cpp
    ../imgui/imgui_tables.cpp
    ../imgui/imgui_widgets.cpp
    ../imgui/backends/imgui_impl_glfw.cpp
    ../imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC ../imgui ../imgui/backends)

# Link ImGui to your application target (replace 'ShaderViewer' with your actual target name if different)
target_link_libraries(ShaderViewer PRIVATE imgui)

# Copy shaders to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/external/tinyobjloader DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(MSVC)
    # MSVC-specific flags
else()
    # GCC/Clang flags for MSYS2
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()
