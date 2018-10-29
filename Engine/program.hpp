#pragma once
#include <string>
#include <filesystem>
#include <optional>
#include <mutex>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <variant>

namespace je
{
	class Buffer;

	class Program
	{
	public:
		Program();
		Program(std::string name, std::filesystem::path vertexPath, std::filesystem::path geometryPath, std::filesystem::path fragmentPath);
		Program(std::string name, std::filesystem::path vertexPath, std::filesystem::path fragmentPath);
		~Program();

		bool upload(bool reload = false);

		void use() const;

		//void setUniform(const std::string & name, std::variant<bool, int, float, glm::vec3, glm::vec4, glm::mat3, glm::mat4> value);

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

		std::filesystem::path _vertexPath;
		std::filesystem::path _geometryPath;
		std::filesystem::path _fragmentPath;

		int _includeLinesOffset = 0;

		std::unordered_map<std::string, std::variant<bool, int, float, glm::vec3, glm::vec4, glm::mat3, glm::mat4>> _uniforms;
		//static std::unordered_map<std::string, int> _ubos;
	//	static std::unordered_map<std::string, int> _samplers;

		static std::mutex _readLock;

		static std::optional<std::filesystem::path> findIncludeFile(const std::string& line, const std::filesystem::path& path, int& commentBlocks);
		std::optional<std::string> readShaderFile(const std::filesystem::path& path);
		std::optional<GLuint> loadAndCompile(GLenum type, const std::filesystem::path& path);
		bool linkProgram(GLuint program);
	};
	
}
