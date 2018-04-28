#pragma once
#include <GL/glew.h>
#include <optional>
#include <vector>

class Texture;

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	inline void bind() const;
	void attachDepthAttachment(const Texture& depthAttachment) const;
	void attachColorAttachment(std::vector<Texture> colorAttachments) const;
	void resize();

private:

	GLuint _handle;
};
