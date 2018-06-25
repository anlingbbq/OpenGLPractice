#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>

#define DrawHouse false

using namespace minor;
class UseGemoetryShader : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "UseGemoetryShader";
	}
	
	void startup()
	{
		Application::startup();

		char* point_vs =
		{
			"#version 430 core									\n"
			"layout(location = 0) in vec2 pos;					\n"
			"layout(location = 1) in vec3 color;				\n"
			"													\n"
			"out VS_OUT {										\n"
			"	vec3 color;										\n"
			"} vs_out;											\n"
			"													\n"
			"void main()										\n"
			"{													\n"
			"	gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);		\n"
			"	vs_out.color = color;							\n"
			"}													\n"
		};

#if DrawHouse == true
		char* point_fs =
		{
			"#version 430 core									\n"
			"out vec4 FragColor;								\n"
			"in vec3 fColor;									\n"
			"													\n"
			"void main()										\n"
			"{													\n"
			"	FragColor = vec4(fColor, 1.0);					\n"
			"}													\n"
		};
		char* draw_house_gs =
		{
			"#version 430 core															\n"
			"layout(points) in;															\n"
			"layout(triangle_strip, max_vertices = 5) out;								\n"
			"																			\n"
			"in VS_OUT {																\n"
			"	vec3 color;																\n"
			"}	gs_in[];																\n"
			"out vec3 fColor;															\n"
			"																			\n"
			"void build_house(vec4 position)											\n"
			"{																			\n"
			"	fColor = gs_in[0].color;												\n"
			"	gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);// 1:bottom-left	\n"
			"	EmitVertex();															\n"
			"	gl_Position = position + vec4(0.2, -0.2, 0.0, 0.0); // 2:bottom-right	\n"
			"	EmitVertex();															\n"
			"	gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0); // 3:top-left		\n"
			"	EmitVertex();															\n"
			"	gl_Position = position + vec4(0.2,  0.2, 0.0, 0.0); // 4:top-right		\n"
			"	EmitVertex();															\n"
			"	gl_Position = position + vec4(0.0,  0.4, 0.0, 0.0); // 5:top			\n"
			"	fColor = vec3(1.0, 1.0, 1.0);											\n"
			"	EmitVertex();															\n"
			"	EndPrimitive();															\n"
			"}																			\n"
			"																			\n"
			"void main() {																\n"
			"	build_house(gl_in[0].gl_Position);										\n"
			"}																			\n"
		};
		_shader = new Shader(point_vs, point_fs, DATA);
		_shader->LoadGeometryShader(draw_house_gs, DATA);
#else
		char* point_fs =
		{
			"#version 430 core									\n"
			"out vec4 FragColor;								\n"
			"													\n"
			"void main()										\n"
			"{													\n"
			"	FragColor = vec4(0.0, 1.0, 0.0, 1.0);			\n"
			"}													\n"
		};
		char* point_gs =
		{
			"#version 430 core									\n"
			"layout(points) in;									\n"
			"layout(points, max_vertices = 1) out;				\n"
			"													\n"
			"void main() {										\n"
			"	gl_Position = gl_in[0].gl_Position;				\n"
			"	EmitVertex();									\n"
			"	EndPrimitive();									\n"
			"}													\n"
		};
		char* draw_line_gs =
		{
			"#version 430 core																\n"
			"layout(points) in;																\n"
			"layout(line_strip, max_vertices = 2) out;										\n"
			"																				\n"
			"void main() {																	\n"
			"	gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0);				\n"
			"	EmitVertex();																\n"
			"																				\n"
			"	gl_Position = gl_in[0].gl_Position + vec4(0.1, 0.0, 0.0, 0.0);				\n"
			"	EmitVertex();																\n"
			"																				\n"
			"	EndPrimitive();																\n"
			"}																				\n"
		};
		_shader = new Shader(point_vs, point_fs, DATA);
		_shader->LoadGeometryShader(draw_line_gs, DATA);
#endif

		glGenBuffers(1, &_vbo);
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

#if DrawHouse == true
		glBufferData(GL_ARRAY_BUFFER, sizeof(_points_color), _points_color, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)(2 * sizeof(float))); 
		glEnableVertexAttribArray(1);
#else
		glBufferData(GL_ARRAY_BUFFER, sizeof(_points), _points, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)0);
		glEnableVertexAttribArray(0);
#endif
		
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	
	void render()
	{
		Application::render();

		_shader->use();
		glDrawArrays(GL_POINTS, 0, 4);
	}

	void shutdown()
	{
		Application::shutdown();
	}
private:
	float _points[8] =
	{
		-0.5f,  0.5f,
		 0.5f,  0.5f,
		-0.5f, -0.5f,
		 0.5f, -0.5f,
	};

	float _points_color[20] =
	{
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
	};

	unsigned int _vao, _vbo;
	Shader* _shader;
};

//DEBUG_MAIN(UseGemoetryShader);