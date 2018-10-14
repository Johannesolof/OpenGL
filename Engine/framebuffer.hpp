#pragma once
#include <GL/glew.h>
#include <vector>
#include <memory>
#include "texture.hpp"

namespace je
{
	class FrameBuffer
	{
	public:
		FrameBuffer();
		~FrameBuffer();

		inline void bind() const;
		inline static void unBind();
		void attachDepthAttachment(const Texture::Data& depthAttachment);
		void attachColorAttachment(const std::vector<Texture::Data>& colorAttachments);
		je::Texture::Data getDepthAttachment() const;
		std::vector<je::Texture::Data> getColorAttachments() const;
		void resize();

	private:

		GLuint _handle;
		je::Texture::Data _depthAttachment;
		std::vector<je::Texture::Data> _colorAttachments;
	};
}
