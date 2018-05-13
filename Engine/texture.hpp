#pragma once

#include <GL/glew.h>
#include <memory>


class Image;

namespace je
{
	struct TexParamters
	{
		int minFilter = GL_LINEAR;
		int magFilter = GL_LINEAR;
		int wrapS = GL_CLAMP;
		int wrapT = GL_CLAMP;
		int wrapR = GL_CLAMP;
	};

	struct Texture
	{
		GLuint       handle{0};
		GLenum       target{0};
		GLint        internalFormat{0};
		GLenum       format{0};
		GLenum       type{0};
		GLsizei      width{0};
		GLsizei      height{0};
		TexParamters texParamters{};
	};


	inline std::shared_ptr<Texture> Create2DTexture(GLenum target, GLint interalFormat, GLenum format, GLenum type, const Image& image, TexParamters paramters = {}, bool generateMipMap = false);
	inline std::shared_ptr<Texture> Create2DTexture(GLenum target, GLint interalFormat, GLenum format, GLenum type, int width, int height, TexParamters paramters = {}, bool generateMipMap = false);

	inline void SetParameterInfo(Texture& tex, TexParamters paramters);
	inline void Bind(Texture& tex);
	inline void UnBind(Texture& tex);

}