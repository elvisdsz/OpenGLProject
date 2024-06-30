cmake_minimum_required (VERSION 3.5)

project (GfxEngine)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++14")
set (source_dir "${PROJECT_SOURCE_DIR}/src")

set (source_files
    "${source_dir}/Application.cpp"
    "${source_dir}/Camera.cpp"
    "${source_dir}/IndexBuffer.cpp"
    "${source_dir}/Renderer.cpp"
    "${source_dir}/Shader.cpp"
    "${source_dir}/Texture.cpp"
    "${source_dir}/VertexArray.cpp"
    "${source_dir}/VertexBuffer.cpp"
    "${source_dir}/VertexBufferLayout.cpp"
    "${source_dir}/test/Test.cpp"
    "${source_dir}/test/TestClearColor.cpp"
    "${source_dir}/test/TestDynamicGeometry.cpp"
    "${source_dir}/test/TestTexture2D.cpp"
    "${source_dir}/test/TestTexture2DBatch.cpp"
    "${source_dir}/test/particles/TestParticleSystem.cpp"
    # Add other necessary source files explicitly
)

file (GLOB_RECURSE header_files "${source_dir}/*.h")

# Debug message to verify the included files
message(STATUS "Source files: ${source_files}")
message(STATUS "Header files: ${header_files}")


#add_executable(GfxEngine src/TestOnly.cpp)
add_executable (GfxEngine ${source_files} ${header_files})

#target_link_libraries(GfxEngine PUBLIC glfw)

# OpenGL_GL_PREFERENCE = GLVND
find_package(OpenGL REQUIRED)

# Find GLEW package
find_package(GLEW REQUIRED)

# STB_IMAGE
add_library(STB_IMAGE STATIC "${source_dir}/vendor/stb_image/stb_image.cpp")

# IMGUI
add_library(IMGUI STATIC 
    "${source_dir}/vendor/imgui/imgui.cpp"
    "${source_dir}/vendor/imgui/imgui_draw.cpp"
    "${source_dir}/vendor/imgui/imgui_widgets.cpp"
    "${source_dir}/vendor/imgui/imgui_impl_glfw.cpp"
    "${source_dir}/vendor/imgui/imgui_impl_opengl3.cpp"
    "${source_dir}/vendor/imgui/imgui_tables.cpp"
)

# Put all libraries into a variable
set(LIBS STB_IMAGE IMGUI glfw GLEW::GLEW OpenGL::GL)

target_include_directories(GfxEngine PUBLIC "${source_dir}/vendor")

# Include directories for GfxEngine
target_include_directories(GfxEngine PUBLIC "${source_dir}" )

target_link_libraries(GfxEngine PUBLIC ${LIBS})