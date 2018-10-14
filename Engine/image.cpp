#include "image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

je::Image::Image()
	: _width(0), _height(0), _channels(0)
{
}

je::LDRImage::LDRImage()
	: Image(), _pixels(nullptr)
{
}

je::LDRImage::~LDRImage()
{
	delete[] _pixels;
}

bool je::LDRImage::load(const std::filesystem::path& filePath)
{
	_pixels = stbi_load(filePath.u8string().c_str(), &_width, &_height, &_channels, 0);
	if (!_pixels)
	{
		printf("Failed to load image %s", filePath.u8string().c_str());
		return false;
	}
	return true;
}

void* je::LDRImage::getPixels() const
{
	return _pixels;
}


je::HDRImage::HDRImage()
	: Image(), _pixels(nullptr)
{
}

je::HDRImage::~HDRImage()
{
	delete[] _pixels;
}

bool je::HDRImage::load(const std::filesystem::path& filePath)
{
	_pixels = stbi_loadf(filePath.u8string().c_str(), &_width, &_height, &_channels, 0);
	if (!_pixels)
	{
		printf("Failed to load image %s", filePath.u8string().c_str());
		return false;
	}
	return true;
}

void* je::HDRImage::getPixels() const
{
	return _pixels;
}