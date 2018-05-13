#include "texture.hpp"

#include "image.hpp"

namespace je {

	std::shared_ptr<Texture> Create2DTexture(GLenum target, GLint interalFormat, GLenum format, GLenum type,
		const Image& image, TexParamters paramters, bool generateMipMap)
	{
		auto tex = std::make_shared<Texture>();
		tex->target = target;
		tex->internalFormat = interalFormat;
		tex->format = format;
		tex->type = type;
		tex->width = image.getWidth();
		tex->height = image.getHeight();
		tex->texParamters = paramters;
		glGenTextures(1, &tex->handle);
		Bind(*tex);
		glTexImage2D(tex->target, 0, tex->internalFormat, tex->width, tex->height, 0, tex->format, tex->type, image.getPixels());
		if (generateMipMap)
			glGenerateMipmap(tex->handle);
		UnBind(*tex);
		return tex;
	}

	std::shared_ptr<Texture> Create2DTexture(GLenum target, GLint interalFormat, GLenum format,
		GLenum type, int width, int height, TexParamters paramters, bool generateMipMap)
	{
		auto tex = std::make_shared<Texture>();
		tex->target = target;
		tex->internalFormat = interalFormat;
		tex->format = format;
		tex->type = type;
		tex->width = width;
		tex->height = height;
		tex->texParamters = paramters;
		glGenTextures(1, &tex->handle);
		Bind(*tex);
		glTexImage2D(tex->target, 0, tex->internalFormat, tex->width, tex->height, 0, tex->format, tex->type, nullptr);
		if (generateMipMap)
			glGenerateMipmap(tex->handle);
		UnBind(*tex);
		return tex;
	}

	void SetParameterInfo(Texture& tex, TexParamters paramters)
	{
		Bind(tex);
		tex.texParamters = paramters;
		glTexParameteri(tex.target, GL_TEXTURE_WRAP_S, paramters.wrapS);
		glTexParameteri(tex.target, GL_TEXTURE_WRAP_T, paramters.wrapT);
		glTexParameteri(tex.target, GL_TEXTURE_MIN_FILTER, paramters.minFilter);
		glTexParameteri(tex.target, GL_TEXTURE_MAG_FILTER, paramters.magFilter);
		UnBind(tex);
	}

	void Bind(Texture& tex)
	{
		glBindTexture(tex.target, tex.handle);
	}

	void UnBind(Texture& tex)
	{
		glBindTexture(tex.target, 0);
	}
}
