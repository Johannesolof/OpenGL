#include "ShapeFactory.hpp"

#include "mesh.hpp"

je::Mesh createCylinder()
{



	return je::Mesh({}, {}, {}, {}, {}, {}, GL_TRIANGLES);
}

je::Mesh createCube()
{
	// Vertices
	const std::vector<GLfloat> vertices = {
		-0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
	};

	const std::vector<GLuint> indices = {
		0, 5, 4,
		0, 1, 5,
		6, 0, 4,
		6, 2, 0,
		2, 1, 0,
		2, 3, 1,
		3, 5, 1,
		3, 7, 5,
		6, 3, 2,
		6, 7, 3,
		4, 7, 6,
		4, 5, 7
	};

	return je::Mesh(vertices, {}, {}, {}, {}, indices, GL_TRIANGLES);
}
