#define STB_IMAGE_IMPLEMENTATION

#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <Minor/Shape/ColorCube.h>
#include <Minor/Shape/TextureCube.h>

using namespace minor;
class MSAA : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "MSAA";
		_camera = ObserveCamera::create(_info.width, _info.height);

		glfwWindowHint(GLFW_SAMPLES, 4);
	}
	
	void startup()
	{
		Application::startup();

		_cube = ColorCube::create(glm::vec3(0, 1, 0), "normal.vs", "constant.fs");
		//_texCube = TextureCube::create("box.png");
		glm::quat r1 = _cube->setRotate(45, glm::vec3(1, 0, 0));
		glm::quat r2 = _cube->setRotate(45, glm::vec3(0, 1, 0));
		_cube->setRotate(r1, r2);
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
	ShapeBase* _cube;
};

//DEBUG_MAIN(MSAA);