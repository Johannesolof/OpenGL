#include "texture.hpp"

#include "image.hpp"

Texture::Texture( GLenum target, GLint interalFormat, GLenum format, GLenum type, const LDRImage& image)
	: _target(target), _internalFormat(interalFormat), _format(format), _type(type)
{
	_handle = 0;
	glGenTextures(1, &_handle);
	
	glTexImage2D(_target, 0, _internalFormat, image.getWidth(), image.getHeight(), 0, _format, _type, image.getPixels());
}
