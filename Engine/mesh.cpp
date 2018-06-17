#include "mesh.hpp"
#include "program.hpp"

namespace je
{
	Mesh::Mesh(std::vector<GLfloat> positions,
		std::vector<GLfloat> normals,
		std::vector<GLfloat> tangents,
		std::vector<GLfloat> bitangents,
		std::vector<GLfloat> texCoords,
		std::vector<GLuint> indices)
		:_vao(0), _vboPositions(0), _vboNormals(0), _vboTangents(0), _vboBitangents(0), _vboTexCoords(0), _ebo(0)
	{
		this->positions = positions;
		this->normals = normals;
		this->tangents = tangents;
		this->bitangents = bitangents;
		this->texCoords = texCoords;
		this->indices = indices;
		setupMesh();
	}

	void Mesh::setupMesh()
	{
		// Generate our vertex array, vertex buffer and element buffer
		glGenVertexArrays(1, &_vao);
		glGenBuffers(1, &_vboPositions);
		glGenBuffers(1, &_vboNormals);
		if (!tangents.empty()) glGenBuffers(1, &_vboTangents);
		if (!bitangents.empty()) glGenBuffers(1, &_vboBitangents);
		if (!texCoords.empty()) glGenBuffers(1, &_vboTexCoords);
		glGenBuffers(1, &_ebo);

		// Bind the vertex array
		glBindVertexArray(_vao);

		// Bind the position vertex buffer and send the vertex data
		glBindBuffer(GL_ARRAY_BUFFER, _vboPositions);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(GLfloat), &positions[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		// Bind the normal vertex buffer and send the vertex data
		glBindBuffer(GL_ARRAY_BUFFER, _vboNormals);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		// If exists bind the tangents vertex buffer and send the vertex data
		if (!tangents.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, _vboTangents);
			glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(GLfloat), &tangents[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		}

		// If exists bind the tangents vertex buffer and send the vertex data
		if (!bitangents.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, _vboBitangents);
			glBufferData(GL_ARRAY_BUFFER, bitangents.size() * sizeof(GLfloat), &bitangents[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		}

		// If exists bind the normal vertex buffer and send the vertex data
		if (!texCoords.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, _vboTexCoords);
			glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), &texCoords[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		}
		// Bind the vertex
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);



		glBindVertexArray(0);
	}

	void Mesh::draw() const
	{
		glBindVertexArray(_vao);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}
}
