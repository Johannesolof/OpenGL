cmake_minimum_required( VERSION 3.9.6 )

project( Engine )

# External libraries
add_subdirectory( ${CMAKE_SOURCE_DIR}/external_src/imgui-1.60 ${CMAKE_CURRENT_BINARY_DIR}/imgui )
set_target_properties(imgui PROPERTIES FOLDER external)

find_package( glm REQUIRED )
find_package( GLEW REQUIRED )
find_package( OpenGL REQUIRED )
find_package( GLFW3 REQUIRED )
find_package( ASSIMP REQUIRED)

# Build and link library
add_library( ${PROJECT_NAME}
    imgui_impl_glfw_gl3.h
    imgui_impl_glfw_gl3.cpp
    engine.hpp
    engine.cpp
	program.hpp
	program.cpp
	errors.hpp
	utils.hpp
	model.hpp
    model.cpp
    mesh.hpp
    mesh.cpp
    base_renderer.hpp
    base_renderer.cpp
	buffer.hpp
	buffer.cpp
    )

target_include_directories( ${PROJECT_NAME}
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/Engine
    ${CMAKE_SOURCE_DIR}/external_src/stb-master
    ${CMAKE_SOURCE_DIR}/external_src/tinyobjloader-1.0.6
    ${CMAKE_SOURCE_DIR}/external_src/boost-1.67.0
    ${GLM_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    ${GLFW3_INCLUDE_DIRS}
	${ASSIMP_INCLUDE_DIRS}
    )

target_link_libraries( ${PROJECT_NAME}
    PUBLIC
    imgui
    ${GLEW_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLFW3_LIBRARIES}
	${ASSIMP_LIBRARIES}	
    )