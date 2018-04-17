#include "buffer.hpp"
#include <cstdio>
#include <cstring>

namespace engine
{
	Buffer::Buffer(GLenum type, const void* data, size_t size)
		: _type(type), _bufferSize(size)
	{
		glGenBuffers(1, &_handle);
		glBindBuffer(type, _handle);
		glBufferData(type, size, data, GL_DYNAMIC_DRAW);
		glBindBuffer(type, 0);
	}

	void Buffer::update(const void* data, size_t size) const
	{
		if (size > _bufferSize)
		{
			printf("Buffer %d too small for the data", _handle);
			return;
		}
		glBindBuffer(_type, _handle);
		GLvoid* p = glMapBuffer(_type, GL_WRITE_ONLY);
		memcpy(p, data, size);
		glUnmapBuffer(_type);
	}

	GLuint Buffer::getHandle() const
	{
		return _handle;
	}

	GLenum Buffer::getType() const
	{
		return _type;
	}
}
