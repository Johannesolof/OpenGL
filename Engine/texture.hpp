#pragma once

#include <GL/glew.h>
#include <memory>



namespace je
{
	class Image;

	class Texture
	{
	public:
		struct Paramters
		{
			int minFilter = GL_LINEAR;
			int magFilter = GL_LINEAR;
			int wrapS = GL_CLAMP;
			int wrapT = GL_CLAMP;
			int wrapR = GL_CLAMP;
		};

		struct Data
		{
			GLuint       handle{ 0 };
			GLenum       target{ 0 };
			GLint        internalFormat{ 0 };
			GLenum       format{ 0 };
			GLenum       type{ 0 };
			GLsizei      width{ 0 };
			GLsizei      height{ 0 };
			Paramters texParamters{};
		} data;


		Texture(GLenum target, GLint interalFormat, GLenum format, GLenum type, const Image& image, Paramters paramters = {}, bool generateMipMap = false);
		Texture(GLenum target, GLint interalFormat, GLenum format, GLenum type, int width, int height, Paramters paramters = {}, bool generateMipMap = false);

		void setParameterInfo(Paramters paramters);
		void bind(int activeTexture);
		void unBind();

	};
}