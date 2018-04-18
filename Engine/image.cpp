#include "image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Image::Image()
	: _width(0), _height(0), _channels(0)
{
}

int Image::getWidth() const
{
	return _width;
}

int Image::getHeight() const
{
	return _height;
}

int Image::getChannels() const
{
	return _channels;
}

LDRImage::LDRImage()
	: Image(), _pixels(nullptr)
{
}

LDRImage::~LDRImage()
{
	delete[] _pixels;
}

bool LDRImage::load(const fs::path& filePath)
{
	_pixels = stbi_load(filePath.u8string().c_str(), &_width, &_height, &_channels, 0);
	if (!_pixels)
	{
		printf("Failed to load image %s", filePath.u8string().c_str());
		return false;
	}
	return true;
}

void* LDRImage::getPixels() const
{
	return _pixels;
}


HDRImage::HDRImage()
	: Image(), _pixels(nullptr)
{
}

HDRImage::~HDRImage()
{
	delete[] _pixels;
}

bool HDRImage::load(const fs::path& filePath)
{
	_pixels = stbi_loadf(filePath.u8string().c_str(), &_width, &_height, &_channels, 0);
	if (!_pixels)
	{
		printf("Failed to load image %s", filePath.u8string().c_str());
		return false;
	}
	return true;
}

void* HDRImage::getPixels() const
{
	return _pixels;
}
