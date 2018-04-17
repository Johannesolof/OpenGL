#pragma once
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace engine
{
	class Program;

	struct Texture
	{
		GLuint id;
		std::string type;
	};

	class Material
	{
	public:
		bool hasBaseTexture;
		Texture baseTexture;
		glm::vec3 baseColor;
		
		bool hasNormalTexture;
		Texture normalTexture;

		bool hasMetallicTexture;
		Texture metallicTexture;
		float metallic;

		bool hasRoughnessTexture;
		Texture roughnessTexture;
		float roughness;
	};

	class Mesh
	{
	public:
		void draw(const Program& program) const;
		Mesh(std::vector<GLfloat> positions, 
			 std::vector<GLfloat> normals,
			 std::vector<GLfloat> tangents,
			 std::vector<GLfloat> bitangents,
			 std::vector<GLfloat> texCoords,
			 std::vector<GLuint> indices);
		~Mesh() = default;

		std::vector<GLfloat> positions;
		std::vector<GLfloat> normals;
		std::vector<GLfloat> tangents;
		std::vector<GLfloat> bitangents;
		std::vector<GLfloat> texCoords;
		std::vector<GLuint> indices;

	private:
        GLuint _vao;
		GLuint _vboPositions, _vboNormals, _vboTangents, _vboBitangents, _vboTexCoords; 
		GLuint _ebo;

        void setupMesh();
	};
}
