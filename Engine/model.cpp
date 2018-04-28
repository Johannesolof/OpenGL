#include "model.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "program.hpp"
#include "texture.hpp"

namespace engine
{
	bool Model::load(const fs::path& path)
	{
		Assimp::Importer importer;
		const auto scene = importer.ReadFile(path.generic_string(), aiProcess_Triangulate | aiProcess_FlipUVs);
		if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
		{
			std::fprintf(stderr, "Error! Failed to load %ls, error code: %s",
				path.filename().c_str(), importer.GetErrorString());
			return false;
		}

		_filepath = path;

		double scaleFactor = 0.0;
		if (scene->mMetaData != nullptr)
			auto res = scene->mMetaData->Get("UnitScaleFactor", scaleFactor);


		processNode(scene->mRootNode, scene);
		return true;

	}

	void Model::draw(const Program& program)
	{
		for (auto &mesh : _meshes)
		{
			mesh.draw(program);
		}
	}

	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<GLfloat> positions;
		std::vector<GLfloat> normals;
		std::vector<GLfloat> tangents;
		std::vector<GLfloat> bitangents;
		std::vector<GLfloat> texCoords;
		std::vector<GLuint>  indices;
		std::vector<Texture> textures;

		for (uint32_t i = 0; i < mesh->mNumVertices; ++i)
		{
			positions.insert(positions.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
			normals.insert(normals.end(), { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z });
			/*if (mesh->HasTangentsAndBitangents())
			{
				vertex.tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
				vertex.bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
			}*/
			if (mesh->HasTextureCoords(0))
			{
				texCoords.insert(texCoords.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
			}
		}

		for (uint32_t i = 0; i < mesh->mNumFaces; ++i)
		{
			const aiFace face = mesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; ++j)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		/*if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
			aiTextureType
		}*/

		return Mesh(positions, normals, tangents, bitangents, texCoords, indices);
	}

	void Model::processNode(aiNode* const node, const aiScene* const scene)
	{
		for (uint32_t i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			_meshes.push_back(processMesh(mesh, scene)); // pair<Mesh, pointer to Material>
		}
		for (uint32_t i = 0; i < node->mNumChildren; ++i)
		{
			processNode(node->mChildren[i], scene);
		}
	}
}
