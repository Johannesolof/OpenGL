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
			HasBaseColor  = 1 << 0,
			HasNormal     = 1 << 1,
			HasMetallic   = 1 << 2,
			HasRoughness  = 1 << 3,
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
			data.flags |= HasRoughness;
		}

		void resetTextures()
		{
			baseColorTexture = 0;
			normalTexture    = 0;
			metallicTexture  = 0;
			rougnessTexture  = 0;
			data.flags &= ~(HasBaseColor | HasNormal | HasMetallic | HasRoughness);
		}
		
	};

	class Mesh
	{
	public:
		Mesh(const std::vector<GLfloat>& positions, 
			 const std::vector<GLfloat>& normals,
			 const std::vector<GLfloat>& tangents,
			 const std::vector<GLfloat>& bitangents,
			 const std::vector<GLfloat>& texCoords,
			 const std::vector<GLuint>&  indices,
			 GLenum mode);
		~Mesh() = default;

		void draw() const;
		
		std::vector<GLfloat> positions;
		std::vector<GLfloat> normals;
		std::vector<GLfloat> tangents;
		std::vector<GLfloat> biTangents;
		std::vector<GLfloat> texCoords;
		std::vector<GLuint> indices;

	private:
        GLuint _vao;
		GLuint _vboPositions, _vboNormals, _vboTangents, _vboBiTangents, _vboTexCoords; 
		GLuint _ebo;
		GLenum _mode;

        void setupMesh();
	};
}
