#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>

using namespace minor;
class Hundred2DQuads : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "Hundred2DQuads";
		_camera = ObserveCamera::create(_info.width, _info.height);
	}
	
	void startup()
	{
		Application::startup();

		_shader = new Shader("hundredQuads.vs", "inputColor.fs");

		glm::vec2 translations[100];
		int index = 0;
		float offset = 0.1f;
		for (int y = -10; y < 10; y += 2)
		{
			for (int x = -10; x < 10; x += 2)
			{
				glm::vec2 translation;
				translation.x = (float)x / 10.0f + offset;
				translation.y = (float)y / 10.0f + offset;
				translations[index++] = translation;
			}
		}
		_shader->use();
		for (unsigned int i = 0; i < 100; i++)
		{
			std::string index = std::to_string(i);
			_shader->setVec2(("offsets[" + index + "]").c_str(), translations[i]);
		}

		glGenBuffers(1, &_vbo);
		glGenVertexArrays(1, &_vao);

		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_quadVertices), _quadVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	
	void render()
	{
		Application::render();

		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
	}

	void shutdown()
	{
		Application::shutdown();
	}
private:
	unsigned int _vao, _vbo;

	float _quadVertices[30] =
	{
		-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
		 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
		-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

		-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
		 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
		 0.05f,  0.05f,  0.0f, 1.0f, 1.0f
	};

	Shader* _shader;
};

//DEBUG_MAIN(Hundred2DQuads);