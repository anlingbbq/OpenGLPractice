#define STB_IMAGE_IMPLEMENTATION

#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <Minor/Shape/ColorCube.h>
#include <Minor/Shape/TextureCube.h>

using namespace minor;
class AntiAliasing : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "AntiAliasing";
		_camera = ObserveCamera::create(_info.width, _info.height);

		glfwWindowHint(GLFW_SAMPLES, 4);
	}
	
	void startup()
	{
		Application::startup();

		//_cube = ColorCube::create(glm::vec3(0, 1, 0), "normal.vs", "constant.fs");
		_texCube = TextureCube::create("box.png");
	}
	
	void render()
	{
		Application::render();

		//_cube->drawShape();
		_texCube->drawShape();
	}

	void shutdown()
	{
		Application::shutdown();
	}
private:
	ColorCube* _cube;
	TextureCube* _texCube;
};

DEBUG_MAIN(AntiAliasing);