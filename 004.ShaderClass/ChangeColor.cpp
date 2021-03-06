#include <Minor/Minor.h>
#include <Minor/Shader.h>

using namespace minor;

class ChangeColor : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "ChangeColor";
	}

	void startup()
	{
		_shader = new Shader("changeColor.vs", "changeColor.fs");

		glGenVertexArrays(1, &_vao);
		glGenBuffers(1, &_vbo);

		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		_shader->use();
	}

	void render()
	{
		float greenValue = (sin(curTime) / 2.0f) + 0.5f;
		_shader->setVec4("ourColor", glm::vec4(0.0f, greenValue, 0.0f, 1.0f));

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

	float _vertices[9] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};

	Shader* _shader;
};

//DEBUG_MAIN(ChangeColor)
