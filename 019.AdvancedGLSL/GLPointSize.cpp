#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>

using namespace minor;
class GLPointSize : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "GLPointSize";
		_info.backgroundColor = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);
		_camera = ObserveCamera::create(_info.width, _info.height);
	}
	
	void startup()
	{
		Application::startup();

		char* point_vs = {
			"#version 430 core																	\n"
			"layout(location = 0) in vec3 pos;													\n"
			"uniform mat4 model;																\n"
			"uniform mat4 view;																	\n"
			"uniform mat4 projection;															\n"
			"void main()																		\n"
			"{																					\n"
			"	gl_Position = projection * view * model * vec4(pos, 1.0);						\n"
			"	// the points we've drawn are rendered larger the more we move away from them	\n"
			"	gl_PointSize = gl_Position.z;													\n"
			"	// gl_PointSize = gl_VertexID * 3;												\n"
			"}																					\n"
		};
		char* point_fs = {
			"#version 430 core								\n"
			"out vec4 fragColor;							\n"
			"void main()									\n"
			"{												\n"
			"	fragColor = vec4(1, 0.3, 0.5, 1);			\n"
			"}												\n"
		};
		_shader = new Shader(point_vs, point_fs, DATA);

		glGenBuffers(1, &_vbo);
		glGenVertexArrays(1, &_vao);

		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_points), _points, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glEnable(GL_PROGRAM_POINT_SIZE);
		//glPointSize(40.0f);
	}
	
	void render()
	{
		Application::render();

		_shader->use();
		_shader->setMat4("view", _camera->getView());
		_shader->setMat4("projection", _camera->getProjection());
		_shader->setMat4("model", glm::mat4(1.0f));
		glDrawArrays(GL_POINTS, 0, 8);
	}

	void shutdown()
	{
		Application::shutdown();
	}
private:
	unsigned int _vao;
	unsigned int _vbo;
	Shader* _shader;

	float _points[21] = 
	{
		-0.6f,  0.6f, -10,
		 0.0f,  1.0f, -10,
		 0.6f,  0.6f, -10,
		 0.0f,  0.0f, -20,
		-0.6f, -0.6f, -10,
		 0.0f, -1.0f, -10,
		 0.6f, -0.6f, -10,
	};
};

//DEBUG_MAIN(GLPointSize);