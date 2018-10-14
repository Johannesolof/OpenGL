#pragma once
#include <filesystem>
#include <assimp/scene.h>
#include <vector>

#include "mesh.hpp"

namespace je
{
	class Program;

	class Model
	{
	public:
		bool load(const std::filesystem::path& path);
		void draw();

	private:
		std::filesystem::path _filepath;
		std::vector<Mesh> _meshes;

		static Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		void processNode(aiNode* const node, const aiScene* const scene);
	};
}
