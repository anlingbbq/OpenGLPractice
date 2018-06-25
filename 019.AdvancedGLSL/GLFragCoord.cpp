#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <Minor/Shape/ColorCube.h>

using namespace minor;
class GLFragCoord : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "GLFragCoord";
		_camera = ObserveCamera::create(_info.width, _info.height);
	}
	
	void startup()
	{
		Application::startup();

		char* cube_vs =
		{
			"#version 430 core													\n"
			"layout(location = 0) in vec3 pos;									\n"
			"																	\n"
			"uniform mat4 model;												\n"
			"uniform mat4 view;													\n"
			"uniform mat4 projection;											\n"
			"																	\n"
			"void main()														\n"
			"{																	\n"
			"	gl_Position = projection * view * model * vec4(pos, 1.0);		\n"
			"}																	\n"
		};
		char* cube_fs =
		{
			"#version 430 core									\n"
			"out vec4 fragColor;								\n"
			"void main()										\n"
			"{													\n"
			"	if (gl_FragCoord.x < 400)						\n"
			"		fragColor = vec4(0.2, 0.3, 0.3, 1.0);		\n"
			"	else											\n"
			"		fragColor = vec4(1.0, 0.3, 0.5, 1.0);		\n"
			"}													\n"
		};
		_cube = ColorCube::create(cube_vs, cube_fs, DATA);
	}
	
	void render()
	{
		Application::render();
		_cube->drawShape();
	}

	void shutdown()
	{
		Application::shutdown();
	}
private:
	ColorCube* _cube;
};

//DEBUG_MAIN(GLFragCoord);