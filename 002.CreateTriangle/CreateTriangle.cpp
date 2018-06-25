#include <Minor/Minor.h>

using namespace minor;
class CreateTriangle : public Application
{
	void init()
	{
		Application::init();
		_info.title = "CreateTriangle";
	}

	void startup()
	{
#pragma region compile vertex shader
		char* vs_source =
		{
			"#version 430 core										\n"
			"layout(location = 0) in vec3 pos;						\n"
			"void main()											\n"
			"{														\n"
			"	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);		\n"
			"}														\n"
		};

		_vso = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(_vso, 1, &vs_source, NULL);
		glCompileShader(_vso);

		int success;
		char infoLog[512];
		glGetShaderiv(_vso, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(_vso, 512, NULL, infoLog);
			printf("vertex shader error: %s", infoLog);
			return;
		}
#pragma endregion

#pragma region compile fragment shader
		char* fs_source =
		{
			"#version 430 core										\n"
			"out vec4 color;										\n"
			"void main()											\n"
			"{														\n"
			"	color = vec4(1.0f, 0.5f, 0.2f, 1.0f);				\n"
			"}														\n"
		};

		_fso = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(_fso, 1, &fs_source, NULL);
		glCompileShader(_fso);

		glGetShaderiv(_fso, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(_fso, 512, NULL, infoLog);
			printf("fragment shader error: %s", infoLog);
			return;
		}
#pragma endregion
		
#pragma region link shader program
		_spo = glCreateProgram();
		glAttachShader(_spo, _vso);
		glAttachShader(_spo, _fso);
		glLinkProgram(_spo);

		glGetProgramiv(_spo, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(_spo, 512, NULL, infoLog);
			printf("link shader program error: %s", infoLog);
			return;
		}

		glDeleteShader(_vso);
		glDeleteShader(_fso);
#pragma endregion
		
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
	}

	void render()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(_spo);
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

	unsigned int _vso;
	unsigned int _fso;
	unsigned int _spo;

	float _vertices[9] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
};

DEBUG_MAIN(CreateTriangle);