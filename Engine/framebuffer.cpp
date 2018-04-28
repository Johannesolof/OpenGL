#include "framebuffer.hpp"

#include "texture.hpp"

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

void FrameBuffer::attachDepthAttachment(const Texture& depthAttachment) const
{
	glBindFramebuffer(GL_FRAMEBUFFER, _handle);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthAttachment.getHandle(), 0);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);  
}

void FrameBuffer::attachColorAttachment(std::vector<Texture> colorAttachments) const
{
	glBindFramebuffer(GL_FRAMEBUFFER, _handle);
	for (size_t i = 0; i < colorAttachments.size(); i++)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorAttachments[i].getHandle(), 0);
	}
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);  
}

void FrameBuffer::resize()
{
	
}
