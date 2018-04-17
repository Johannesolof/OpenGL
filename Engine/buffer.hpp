#pragma once
#include <GL/glew.h>

namespace engine
{
	class Buffer
	{
	public:
		explicit Buffer(GLenum type, const void* data, size_t size);

		void update(const void* data, size_t size) const;

		GLuint getHandle() const;
		GLenum getType() const;

	private:
		GLuint _handle = 0;
		GLenum _type;
		size_t _bufferSize;
	};
}
