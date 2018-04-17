#include "program.hpp"
#include <optional>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <queue>
#include <utility>
#include <boost/algorithm/string.hpp>
#include "buffer.hpp"

namespace engine
{

	std::unordered_map<std::string, int> Program::_ubos;

	Program::Program(std::string name, fs::path vertexPath, fs::path geometryPath, fs::path fragmentPath)
		:_name(std::move(name)), _program(0), _vertexPath(std::move(vertexPath)),
		_geometryPath(std::move(geometryPath)), _fragmentPath(std::move(fragmentPath))
	{
		upload(false);
	}

	Program::Program(std::string name, fs::path vertexPath, fs::path fragmentPath)
		: Program(std::move(name), std::move(vertexPath), "", std::move(fragmentPath))
	{
	}

	Program::~Program()
	{
		glDeleteProgram(_program);
	}

	std::optional<fs::path> Program::findIncludeFile(const std::string& line, const fs::path& path, int& commentBlocks)
	{
		size_t commentBlockBeginIndex = line.find("/*");
		size_t commentBlockEndIndex = line.find("*/");
		size_t lineCommentIndex = line.find("//");
		size_t includeIndex = line.find("#include ");

		// Change not found to line length instead of -1
		if (commentBlockBeginIndex == std::string::npos) commentBlockBeginIndex = line.length();
		else commentBlocks++;
		if (commentBlockEndIndex == std::string::npos) commentBlockEndIndex = line.length();
		else commentBlocks--;
		if (lineCommentIndex == std::string::npos) lineCommentIndex = line.length();
		if (includeIndex == std::string::npos) includeIndex = line.length();

		if (commentBlocks == 0 && lineCommentIndex > includeIndex) // If #inlcude is not commented out
		{
			fs::path includeFile = path.parent_path(); // Get directory of the current file

			// Find the filename
			std::vector<std::string> partsOfLine;
			boost::split(partsOfLine, line, boost::is_any_of(" "), boost::token_compress_on);
			if (partsOfLine.size() > 1)
			{
				includeFile.append(partsOfLine[1]); // Filename should be the second word on the line (after include)
				return includeFile;

			}
		}
		return std::nullopt;
	}


	std::optional<std::string> Program::readShaderFile(const fs::path& path)
	{
		std::ifstream in(path.u8string());
		std::string source = "";

		int commentBlocks = 0;

		for (std::string line; std::getline(in, line);) // TODO: Handle block comments on the same line.
		{
			auto includeFile = findIncludeFile(line, path, commentBlocks); // Find include path

			if (includeFile && std::find(_includePaths.begin(), _includePaths.end(), (*includeFile).u8string()) == _includePaths.end()) // If exist and has not been included already
			{
				_includePaths.emplace_back((*includeFile).u8string());
				auto includeSource = readShaderFile(*includeFile); // Read include file
				if (includeSource) // If succes add it to the current file
				{
					source.append(*includeSource);
					_includeLinesOffset = std::count((*includeSource).begin(), (*includeSource).end(), '\n');
					continue;
				}
			}

			source.append(line).append("\n");
		}

		if (source.empty())
		{
			return std::nullopt;
		}
		return source;
	}

	std::optional<GLuint> Program::loadAndCompile(GLenum type, const fs::path& path)
	{

		auto source = readShaderFile(path);
		if (!source)
		{
			return std::nullopt;
		}

		const GLuint id = glCreateShader(type);

		const char* c_path = (*source).c_str();
		glShaderSource(id, 1, &c_path, nullptr);

		glCompileShader(id);

		int success = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			int logLength = 0;
			int charsWritten = 0;
			std::string err;

			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

			if (logLength > 0) {
				char *cLog = new char[logLength];
				glGetShaderInfoLog(id, logLength, &charsWritten, cLog);
				err = cLog;
				delete[] cLog;
			}
			fprintf(stderr, "ShaderError: %s", err.c_str());
			return std::nullopt;
		}
		return id;
	}

	bool Program::linkProgram(const GLuint program)
	{
		glLinkProgram(program);
		int success = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			int logLength = 0;
			int charsWritten = 0;
			std::string err;

			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

			if (logLength > 0) {
				char *cLog = new char[logLength];
				glGetProgramInfoLog(program, logLength, &charsWritten, cLog);
				err = cLog;
				delete[] cLog;
			}
			fprintf(stderr, "LinkError: %s", err.c_str());
			return false;
		}
		return true;
	}

	bool Program::upload(const bool reload)
	{
		const GLuint tempProgram = glCreateProgram();

		auto vs = loadAndCompile(GL_VERTEX_SHADER, _vertexPath);
		auto gs = loadAndCompile(GL_GEOMETRY_SHADER, _geometryPath);
		auto fs = loadAndCompile(GL_FRAGMENT_SHADER, _fragmentPath);

		if (vs) glAttachShader(tempProgram, *vs);
		if (gs)	glAttachShader(tempProgram, *gs);
		if (fs)	glAttachShader(tempProgram, *fs);

		const auto linkSuccess = linkProgram(tempProgram);
		if (linkSuccess)
			_program = tempProgram;

		if (vs && linkSuccess) glDetachShader(tempProgram, *vs);
		if (gs && linkSuccess) glDetachShader(tempProgram, *gs);
		if (fs && linkSuccess) glDetachShader(tempProgram, *fs);

		if (vs) glDeleteShader(*vs);
		if (gs) glDeleteShader(*gs);
		if (fs) glDeleteShader(*fs);

		return linkSuccess || reload;
	}

	//GLuint Program::createShaderStorageBufferObject(const GLvoid* data)
	//{
	//	GLuint ssbo = 0;
	//	glGenBuffers(1, &ssbo);
	//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	//	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(data), &data, GL_DYNAMIC_COPY);
	//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	//	return ssbo;
	//}

	void Program::use() const
	{
		glUseProgram(_program);
	}

	void Program::setUniform(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(_program, name.c_str()), static_cast<int>(value));
	}

	void Program::setUniform(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(_program, name.c_str()), value);
	}

	void Program::setUniform(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(_program, name.c_str()), value);
	}

	void Program::setUniform(const std::string& name, glm::vec3 value) const
	{
		glUniform3fv(glGetUniformLocation(_program, name.c_str()), 1, glm::value_ptr(value));
	}

	void Program::setUniform(const std::string& name, glm::mat3 value) const
	{
		glUniformMatrix3fv(glGetUniformLocation(_program, name.c_str()), 1, false, glm::value_ptr(value));
	}

	void Program::setUniform(const std::string& name, glm::vec4 value) const
	{
		glUniform4fv(glGetUniformLocation(_program, name.c_str()), 1, glm::value_ptr(value));
	}

	void Program::setUniform(const std::string& name, glm::mat4 value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(_program, name.c_str()), 1, false, glm::value_ptr(value));
	}

	bool Program::bindUniformBuffer(std::string name, const Buffer& buffer) const
	{
		if (buffer.getType() != GL_UNIFORM_BUFFER)
		{
			printf("Buffer is not a GL_UNIFORM_BUFFER, actual buffer type: %d", buffer.getType());
			return false;
		}

		const GLuint uniformIndex = glGetUniformBlockIndex(_program, name.c_str());
		if (uniformIndex == GL_INVALID_INDEX)
		{
			printf("Could not find uniform buffer named '%s' in shader program '%s'", name.c_str(), _name.c_str());
			return false;
		}

		const auto it = _ubos.insert({name, _ubos.size()}).first;
		glBindBufferBase(buffer.getType(), it->second, buffer.getHandle());
		glUniformBlockBinding(_program, uniformIndex, it->second);
		return true;
	}
}

