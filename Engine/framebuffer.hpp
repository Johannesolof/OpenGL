#pragma once
#include <GL/glew.h>
#include <optional>
#include <vector>
#include <memory>

class Texture;

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	inline void bind() const;
	inline static void unBind();
	void attachDepthAttachment(const std::shared_ptr<Texture>& depthAttachment);
	void attachColorAttachment(const std::vector<std::shared_ptr<Texture>>& colorAttachments);
	std::shared_ptr<Texture> getDepthAttachment() const;
	std::vector<std::shared_ptr<Texture>> getColorAttachments() const;
	void resize();

private:

	GLuint _handle;
	std::shared_ptr<Texture> _depthAttachment;
	std::vector<std::shared_ptr<Texture>> _colorAttachments;
};
