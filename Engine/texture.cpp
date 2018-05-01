#include "texture.hpp"

#include "image.hpp"

Texture::Texture( GLenum target, GLint interalFormat, GLenum format, GLenum type, const Image& image, TexParamters paramters, bool generateMipMap)
	: _target(target), _internalFormat(interalFormat), _format(format), _type(type), _width(image.getWidth()), _height(image.getHeight())
{
	_handle = 0;
	glGenTextures(1, &_handle);
	bind();
	glTexImage2D(_target, 0, _internalFormat, _width, _height, 0, _format, _type, image.getPixels());
	if (generateMipMap)
		glGenerateMipmap(GL_TEXTURE_2D);
	unBind();
}

Texture::Texture(GLenum target, GLint interalFormat, GLenum format, GLenum type, int width, int height, TexParamters paramters, bool generateMipMap)
	: _target(target), _internalFormat(interalFormat), _format(format), _type(type), _width(width), _height(height)
{
	_handle = 0;
	glGenTextures(1, &_handle);
	bind();
	glTexImage2D(_target, 0, _internalFormat, _width, _height, 0, _format, _type, nullptr);
	setParameterInfo(paramters);
	if (generateMipMap)
		glGenerateMipmap(GL_TEXTURE_2D);
	unBind();
}

void Texture::setParameterInfo(TexParamters paramters)
{
	bind();
	_texParamters = paramters;
	glTexParameteri(_target, GL_TEXTURE_WRAP_S, _texParamters.wrapS);	
	glTexParameteri(_target, GL_TEXTURE_WRAP_T, _texParamters.wrapT);
	glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, _texParamters.minFilter);
	glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, _texParamters.magFilter);
	unBind();
}

void Texture::bind() const
{
	glBindTexture(_target, _handle);
}

void Texture::unBind() const
{
	glBindTexture(_target, 0);
}

GLuint Texture::getHandle() const
{
	return _handle;
}
