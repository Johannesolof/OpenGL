#pragma once
#include <string>
#include <filesystem>
#include <optional>
#include <mutex>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <unordered_map>

namespace fs = std::experimental::filesystem;

namespace je
{
	class Buffer;

	class Program
	{
	public:
		Program();
		Program(std::string name, fs::path vertexPath, fs::path geometryPath, fs::path fragmentPath);
		Program(std::string name, fs::path vertexPath, fs::path fragmentPath);
		~Program();

		bool upload(bool reload = false);

		void use() const;

		void setUniform(const std::string &name, bool value) const;
		void setUniform(const std::string &name, int value) const;
		void setUniform(const std::string &name, float value) const;
		void setUniform(const std::string &name, glm::vec3 value) const;
		void setUniform(const std::string &name, glm::mat3 value) const;
		void setUniform(const std::string &name, glm::vec4 value) const;
		void setUniform(const std::string &name, glm::mat4 value) const;

		bool bindUniformBuffer(std::string name, const Buffer& buffer) const;
		bool bindSampler(std::string name, GLuint sampler);


	private:
		std::string _name;
		GLuint _program;

		fs::path _vertexPath;
		fs::path _geometryPath;
		fs::path _fragmentPath;

		int _includeLinesOffset = 0;

		//static std::unordered_map<std::string, int> _ubos;
	//	static std::unordered_map<std::string, int> _samplers;

		static std::mutex _readLock;

		static std::optional<fs::path> findIncludeFile(const std::string& line, const fs::path& path, int& commentBlocks);
		std::optional<std::string> readShaderFile(const fs::path& path);
		std::optional<GLuint> loadAndCompile(GLenum type, const fs::path& path);
		bool linkProgram(GLuint program);
	};
	
}
