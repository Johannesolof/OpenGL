#pragma once
#include <GL/glew.h>

namespace je
{
	class Buffer
	{
	public:
		explicit Buffer(GLenum type, const void* data, size_t size);

		void update(const void* data, size_t size) const;

		GLuint handle = 0;
		GLenum type;
		size_t bufferSize;
	};
}
