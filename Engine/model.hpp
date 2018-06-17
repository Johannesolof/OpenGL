#pragma once
#include <filesystem>
#include <assimp/scene.h>
#include <vector>

#include "mesh.hpp"

namespace fs = std::experimental::filesystem;

namespace je
{
	class Program;

	class Model
	{
	public:
		bool load(const fs::path& path);
		void draw();

	private:
		fs::path _filepath;
		std::vector<Mesh> _meshes;

		static Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		void processNode(aiNode* const node, const aiScene* const scene);
	};
}
