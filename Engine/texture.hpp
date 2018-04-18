#pragma once

#include <GL/glew.h>
#include <filesystem>

class LDRImage;
namespace fs = std::experimental::filesystem;

class Texture
{
public:

	struct TexParamters
	{
		int minFilter;
		int magFilter;
		int wrapS;
		int wrapT;
		int wrapR;
	};


	Texture(GLenum target, GLint interalFormat, GLenum format, GLenum type, const LDRImage& image);

	void setParameterInfo(TexParamters paramters);

private:
	GLuint _handle;
	GLenum _target;
	GLint _internalFormat;
	GLenum _format;
	GLenum _type;
};