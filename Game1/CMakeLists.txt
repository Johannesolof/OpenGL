cmake_minimum_required( VERSION 3.9.6 )

project( Game1 )

# Build and link executable.
add_executable( ${PROJECT_NAME}
    main.cpp
    App.hpp
    App.cpp
	camera.hpp
    camera.cpp
	input.hpp
    input.cpp
    Shapes/ShapeFactory.hpp
    Shapes/ShapeFactory.cpp
)

target_link_libraries( ${PROJECT_NAME}
    Engine
)
config_build_output()