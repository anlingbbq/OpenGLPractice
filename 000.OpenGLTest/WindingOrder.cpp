#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>

using namespace minor;
class WindingOrder : public Application
{
	void init()
	{
		Application::init();
		_info.title = "CreateTriangle";
		_camera = ObserveCamera::create(_info.width, _info.height);
	}

	void startup()
	{
		char* vs_source =
		{
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

		char* fs_source =
		{
			"#version 430 core										\n"
			"out vec4 color;										\n"
			"void main()											\n"
			"{														\n"
			"	color = vec4(1.0f, 0.5f, 0.2f, 1.0f);				\n"
			"}														\n"
		};

		_shader = new Shader(vs_source, fs_source, DATA);

#pragma region vertex buffer and vertex array
		glGenBuffers(1, &_vbo);
		glGenVertexArrays(1, &_vao);

		glBindVertexArray(_vao);

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
#pragma endregion

		glEnable(GL_CULL_FACE);
	}

	void render()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		_shader->use();
		_shader->setMat4("view", _camera->getView());
		_shader->setMat4("projection", _camera->getProjection());
		glm::mat4 model = glm::mat4(1);
		_shader->setMat4("model", model);
		glBindVertexArray(_vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void shutdown()
	{
		glDeleteVertexArrays(1, &_vao);
		glDeleteBuffers(1, &_vbo);
	}

private:
	unsigned int _vbo;
	unsigned int _vao;
	
	Shader* _shader;

	float _vertices[9] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f,
	};
};

//DEBUG_MAIN(WindingOrder);