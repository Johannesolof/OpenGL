#include "framebuffer.hpp"

namespace je
{

	FrameBuffer::FrameBuffer()
		: _handle(0)
	{
		glGenFramebuffers(1, &_handle);
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &_handle);
	}

	void FrameBuffer::bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _handle);
	}

	void FrameBuffer::unBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::attachDepthAttachment(const Texture::Data& depthAttachment)
	{
		_depthAttachment = depthAttachment;
		glBindFramebuffer(GL_FRAMEBUFFER, _handle);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthAttachment.handle, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::attachColorAttachment(const std::vector<Texture::Data>& colorAttachments)
	{
		_colorAttachments = colorAttachments;
		glBindFramebuffer(GL_FRAMEBUFFER, _handle);
		for (size_t i = 0; i < colorAttachments.size(); i++)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(i), GL_TEXTURE_2D, colorAttachments[i].handle, 0);
		}
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	je::Texture::Data FrameBuffer::getDepthAttachment() const
	{
		return _depthAttachment;
	}

	std::vector<je::Texture::Data> FrameBuffer::getColorAttachments() const
	{
		return _colorAttachments;
	}

	void FrameBuffer::resize()
	{

	}
}