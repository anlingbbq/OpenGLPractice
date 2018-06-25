#ifndef __Rectangle_H__
#define __Rectangle_H__

#include "MRectangle.h"

namespace minor
{
	static char* color_rect_vs = {
		"#version 430 core												\n"
		"layout(location = 0) in vec3 pos;								\n"
		"uniform mat4 model;											\n"
		"uniform mat4 view;												\n"
		"uniform mat4 projection;										\n"
		"void main()													\n"
		"{																\n"
		"	gl_Position = projection * view * model * vec4(pos, 1.0);	\n"
		"}																\n"
	};

	static char* color_rect_fs = {
		"#version 430 core									\n"
		"uniform vec3 inputColor;							\n"
		"out vec4 fragColor;								\n"
		"													\n"
		"void main()										\n"
		"{													\n"
		"	fragColor = vec4(inputColor, 1);				\n"
		"}													\n"
	};

	static float rect_vertices[12] =
	{
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	static unsigned int rect_indices[6] =
	{
		0, 1, 3,
		1, 2, 3
	};

	class ColorRectangle : public MRectangle
	{
	public:
		static ColorRectangle* create(glm::vec3 color)
		{
			ColorRectangle* rect = new ColorRectangle(rect_vertices, rect_indices, sizeof(rect_vertices), sizeof(rect_indices));
			rect->init();
			rect->setColor(color);

			return rect;
		}

		ColorRectangle(float* vertices, unsigned int* indices, int verSize, int indSize) : MRectangle(vertices, indices, verSize, indSize) {}

		virtual void init()
		{
			MRectangle::init();

			_trans = glm::mat4(1.0f);
			_changeTrans = true;

			_shader = new Shader(color_rect_vs, color_rect_fs, DATA);
		}

		virtual void interpret()
		{
			glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
		}

		virtual void setColor(glm::vec3 color)
		{
			_color = color;
			_shader->use();
			_shader->setVec3("inputColor", _color);
		}

		virtual void drawShape()
		{
			glBindVertexArray(_vao);

			_shader->use();
			changeView();
			changeModel();

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		virtual void clear()
		{
			glDeleteVertexArrays(1, &_vao);
			glDeleteBuffers(1, &_vbo);
			glDeleteBuffers(1, &_ebo);

			delete _shader;
			delete this;
		}

	protected:
		glm::vec3 _color;
	};
};

#endif // !__Rectangle_H__

