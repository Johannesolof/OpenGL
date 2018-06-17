#include "buffer.hpp"
#include <cstdio>
#include <cstring>

namespace je
{
	Buffer::Buffer(GLenum type, const void* data, size_t size)
		: type(type), bufferSize(size)
	{
		glGenBuffers(1, &handle);
		glBindBuffer(type, handle);
		glBufferData(type, size, data, GL_DYNAMIC_DRAW);
		glBindBuffer(type, 0);
	}

	void Buffer::update(const void* data, size_t size) const
	{
		if (size > bufferSize)
		{
			printf("Buffer %d too small for the data", handle);
			return;
		}
		glBindBuffer(type, handle);
		GLvoid* p = glMapBuffer(type, GL_WRITE_ONLY);
		memcpy(p, data, size);
		glUnmapBuffer(type);
	}
}
