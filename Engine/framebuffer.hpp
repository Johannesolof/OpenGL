#pragma once
#include <GL/glew.h>
#include <optional>
#include <vector>
#include <memory>

namespace je
{
	struct Texture;

	class FrameBuffer
	{
	public:
		FrameBuffer();
		~FrameBuffer();

		inline void bind() const;
		inline static void unBind();
		void attachDepthAttachment(const std::shared_ptr<je::Texture>& depthAttachment);
		void attachColorAttachment(const std::vector<std::shared_ptr<je::Texture>>& colorAttachments);
		std::shared_ptr<je::Texture> getDepthAttachment() const;
		std::vector<std::shared_ptr<je::Texture>> getColorAttachments() const;
		void resize();

	private:

		GLuint _handle;
		std::shared_ptr<je::Texture> _depthAttachment;
		std::vector<std::shared_ptr<je::Texture>> _colorAttachments;
	};
}