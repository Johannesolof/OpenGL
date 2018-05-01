#pragma once

#include <GL/glew.h>
#include <filesystem>

class Image;
namespace fs = std::experimental::filesystem;

class Texture
{
public:

	struct TexParamters
	{
		int minFilter = GL_LINEAR;
		int magFilter = GL_LINEAR;
		int wrapS = GL_CLAMP;
		int wrapT = GL_CLAMP;
		int wrapR = GL_CLAMP;
	};


	Texture(GLenum target, GLint interalFormat, GLenum format, GLenum type, const Image& image, TexParamters paramters = {}, bool generateMipMap = false);
	Texture(GLenum target, GLint interalFormat, GLenum format, GLenum type, int width, int height, TexParamters paramters = {}, bool generateMipMap = false);

	void setParameterInfo(TexParamters paramters);
	void bind() const;
	void unBind() const;

	inline GLuint getHandle() const;

private:
	GLuint _handle;
	GLenum _target;
	GLint _internalFormat;
	GLenum _format;
	GLenum _type;
	GLsizei _width;
	GLsizei _height;
	TexParamters _texParamters;
};