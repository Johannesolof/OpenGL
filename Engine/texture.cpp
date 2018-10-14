#include "texture.hpp"

#include "image.hpp"

je::Texture::Texture(GLenum target, GLint interalFormat, GLenum format, GLenum type, const Image& image,
	Paramters paramters, bool generateMipMap)
{
	data.target = target;
	data.internalFormat = interalFormat;
	data.format = format;
	data.type = type;
	data.width = image._width;
	data.height = image._height;
	data.texParamters = paramters;
	glGenTextures(1, &data.handle);
	glBindTexture(data.target, data.handle);
	glTexImage2D(data.target, 0, data.internalFormat, data.width, data.height, 0, data.format, data.type, image.getPixels());
	if (generateMipMap)
		glGenerateMipmap(data.handle);
	glBindTexture(data.target, 0);
}

je::Texture::Texture(GLenum target, GLint interalFormat, GLenum format, GLenum type, int width, int height,
	Paramters paramters, bool generateMipMap)
{
	data.target = target;
	data.internalFormat = interalFormat;
	data.format = format;
	data.type = type;
	data.width = width;
	data.height = height;
	data.texParamters = paramters;
	glGenTextures(1, &data.handle);
	glBindTexture(data.target, data.handle);
	glTexImage2D(data.target, 0, data.internalFormat, data.width, data.height, 0, data.format, data.type, nullptr);
	if (generateMipMap)
		glGenerateMipmap(data.handle);
	glBindTexture(data.target, 0);
}

void je::Texture::setParameterInfo(Paramters paramters)
{	
	bind(0);
	data.texParamters = paramters;
	glTexParameteri(data.target, GL_TEXTURE_WRAP_S, paramters.wrapS);
	glTexParameteri(data.target, GL_TEXTURE_WRAP_T, paramters.wrapT);
	glTexParameteri(data.target, GL_TEXTURE_MIN_FILTER, paramters.minFilter);
	glTexParameteri(data.target, GL_TEXTURE_MAG_FILTER, paramters.magFilter);
	unBind();
}

void je::Texture::bind(int activeTexture)
{
	glActiveTexture(activeTexture);
	glBindTexture(data.target, data.handle);
}

void je::Texture::unBind()
{
	glBindTexture(data.target, 0);
}
