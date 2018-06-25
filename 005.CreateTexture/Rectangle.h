#ifndef __Rectangle_H__
#define __Rectangle_H__

#include "ShapeBase.h"

namespace minor
{
	class Rectangle : public ShapeBase
	{
	public:
		static Rectangle* create(float* vertices, unsigned int* indices, int verSize, int indSize)
		{
			Rectangle* rect = new Rectangle(vertices, indices, verSize, indSize);
			return rect;
		}

		Rectangle(float* vertices, unsigned int* indices, int verSize, int indSize)
		{
			_vertices = vertices;
			_indices = indices;

			_verSize = verSize;
			_indSize = indSize;
		}

		virtual void allot()
		{
			glGenVertexArrays(1, &_vao);
			glGenBuffers(1, &_vbo);
			glGenBuffers(1, &_ebo);

			glBindVertexArray(_vao);

			glBindBuffer(GL_ARRAY_BUFFER, _vbo);
			glBufferData(GL_ARRAY_BUFFER, _verSize, _vertices, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indSize, _indices, GL_STATIC_DRAW);
		}

		virtual void unbind()
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindVertexArray(0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		void bindVertexArray()
		{
			glBindVertexArray(_vao);
		}

		virtual void clear()
		{
			glDeleteVertexArrays(1, &_vao);
			glDeleteBuffers(1, &_vbo);
			glDeleteBuffers(1, &_ebo);

			delete this;
		}

	protected:
		unsigned int _ebo;

		int _indSize;
		unsigned int* _indices;
	};
};

#endif // !__Rectangle_H__

