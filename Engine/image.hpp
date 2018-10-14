#pragma once

#include <filesystem>

namespace je
{
	class Image
	{
	public:
		virtual ~Image() = default;

		virtual bool load(const std::filesystem::path& filePath) = 0;

		virtual void* getPixels() const = 0;
		
		int _width;
		int _height;
		int _channels;

	protected:
		Image();
		Image(const Image& other) = default;
		Image(Image&& other) noexcept = default;
		Image& operator=(const Image& other) = default;
		Image& operator=(Image&& other) noexcept = default;


	};

	class LDRImage : public Image
	{
	public:
		LDRImage();
		LDRImage(const LDRImage& other) = default;
		LDRImage(LDRImage&& other) noexcept = default;
		LDRImage& operator=(const LDRImage& other) = default;
		LDRImage& operator=(LDRImage&& other) noexcept = default;
		virtual ~LDRImage();

		bool load(const std::filesystem::path& filePath) override;
		inline void* getPixels() const override;

	private:
		unsigned char* _pixels;
	};

	class HDRImage : public Image
	{
	public:
		HDRImage();
		HDRImage(const HDRImage& other) = default;
		HDRImage(HDRImage&& other) noexcept = default;
		HDRImage& operator=(const HDRImage& other) = default;
		HDRImage& operator=(HDRImage&& other) noexcept = default;
		virtual ~HDRImage();

		bool load(const std::filesystem::path& filePath) override;
		inline void* getPixels() const override;

	private:
		float* _pixels;
	};
}