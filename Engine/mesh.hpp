#pragma once
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace je
{
	class Program;


	struct Material
	{
		enum FlagsType
		{
			HasBaseColor = 1 << 0,
			HasNormal    = 1 << 1,
			HasMetallic  = 1 << 2,
			HasRougness  = 1 << 3,
		};

		struct Data
		{
			GLuint flags        = 0x00000000u;
			glm::vec4 baseColor = glm::vec4(240.f, 177.f, 188.f, 0.f) / 255.f;
			float metallic      = 0.f;
			float rougness      = 0.7f;
		} data;

		GLuint baseColorTexture {0};
		GLuint normalTexture    {0};
		GLuint metallicTexture  {0};
		GLuint rougnessTexture  {0};

		void setBaseColorTexture(GLuint textureHandle)
		{
			baseColorTexture = textureHandle;
			data.flags |= HasBaseColor;
		}

		void setNormalTexture(GLuint textureHandle)
		{
			normalTexture = textureHandle;
			data.flags |= HasNormal;
		}

		void setMetallicTexture(GLuint textureHandle)
		{
			metallicTexture = textureHandle;
			data.flags |= HasMetallic;
		}
		
		void setRoughnessTexture(GLuint textureHandle)
		{
			rougnessTexture = textureHandle;
			data.flags |= HasRougness;
		}

		void resetTextures()
		{
			baseColorTexture = 0;
			normalTexture    = 0;
			metallicTexture  = 0;
			rougnessTexture  = 0;
			data.flags &= ~(HasBaseColor | HasNormal | HasMetallic | HasRougness);
		}
		
	};

	class Mesh
	{
	public:
		Mesh(std::vector<GLfloat> positions, 
			 std::vector<GLfloat> normals,
			 std::vector<GLfloat> tangents,
			 std::vector<GLfloat> bitangents,
			 std::vector<GLfloat> texCoords,
			 std::vector<GLuint> indices);
		~Mesh() = default;

		void draw() const;
		
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
