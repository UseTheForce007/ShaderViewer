cmake_minimum_required(VERSION 3.5)
cmake_policy(VERSION 3.5)
project(ShaderViewer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler selection
if(DEFINED ENV{CC} AND DEFINED ENV{CXX})
    # Use environment variables if set (e.g., CC=clang CXX=clang++)
    set(CMAKE_C_COMPILER $ENV{CC})
    set(CMAKE_CXX_COMPILER $ENV{CXX})
    message(STATUS "Using compilers from environment: CC=$ENV{CC}, CXX=$ENV{CXX}")
elseif(EXISTS "/usr/bin/clang" AND EXISTS "/usr/bin/clang++")
    # Default to Clang if available on Ubuntu
    set(CMAKE_C_COMPILER "clang")
    set(CMAKE_CXX_COMPILER "clang++")
    message(STATUS "Using Clang compilers")
endif()

# Find packages
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)

# Find GLFW
pkg_check_modules(GLFW REQUIRED glfw3)

# Find X11 libraries (required for GLFW on Linux)
find_package(X11 REQUIRED)

# Find GLM (header-only library)

# Add external dependencies
# add_subdirectory(external)

# Add source files
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Shader.cpp
    src/ObjModel.cpp # Using ObjModel for OBJ loading
    src/Camera.cpp

    # src/Model.cpp  # Commented out, not used for OBJ test
)

add_library(glad STATIC external/glad/gl.c)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glm # Add GLM headers
    ${CMAKE_CURRENT_SOURCE_DIR}/external/tinyobjloader
    ${GLFW_INCLUDE_DIRS}
)

# Add GLM include directory if found
if(glm_FOUND)
    target_include_directories(${PROJECT_NAME} PRIVATE ${glm_INCLUDE_DIRS})
elseif(GLM_INCLUDE_DIR)
    target_include_directories(${PROJECT_NAME} PRIVATE ${GLM_INCLUDE_DIR})
endif()

target_include_directories(glad PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/external
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${GLFW_LIBRARIES}
    glad
    OpenGL::GL
    ${CMAKE_DL_LIBS} # Required for dynamic loading on Linux
    ${X11_LIBRARIES}
    ${X11_Xrandr_LIB}
    ${X11_Xinput_LIB}
    ${X11_Xcursor_LIB}
    ${X11_Xxf86vm_LIB}
    pthread
)

# Add ImGui library from external/imgui (if it exists)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui.cpp")
    add_library(imgui
        external/imgui/imgui.cpp
        external/imgui/imgui_draw.cpp
        external/imgui/imgui_tables.cpp
        external/imgui/imgui_widgets.cpp
        external/imgui/backends/imgui_impl_glfw.cpp
        external/imgui/backends/imgui_impl_opengl3.cpp
    )
    target_include_directories(imgui PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/backends
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE imgui)
    message(STATUS "ImGui found and added to build")
else()
    message(WARNING "ImGui not found at external/imgui/. Please clone it there.")
endif()

# Copy resources to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Only copy tinyobjloader if it exists as a separate directory
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/tinyobjloader" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/external/tinyobjloader")
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/external/tinyobjloader DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

# Compiler-specific flags
if(MSVC)
    # MSVC-specific flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    # GCC/Clang flags for Linux/Ubuntu
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

    # Clang-specific optimizations
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message(STATUS "Using Clang-specific optimizations")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -fsanitize=address")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
        set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address")
    endif()
endif()
