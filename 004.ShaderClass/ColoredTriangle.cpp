#include <Minor/Minor.h>
#include <Minor/Shader.h>

using namespace minor;

class ColoredTriangle : public Application
{
	void init()
	{
		Application::init();
		_info.title = "ColoredTriangle";
	}

	void startup()
	{
		_shader = new Shader("colored.vs", "colored.fs");

		glGenVertexArrays(1, &_vao);
		glGenBuffers(1, &_vbo);

		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		_shader->use();
	}

	void render()
	{
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void shutdown()
	{
		glDeleteVertexArrays(1, &_vao);
		glDeleteBuffers(1, &_vbo);

		delete _shader;
	}

private:
	unsigned int _vao;
	unsigned int _vbo;

	float _vertices[18] =
	{
		// position			 // color
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
	};

	Shader* _shader;
};

DEBUG_MAIN(ColoredTriangle);