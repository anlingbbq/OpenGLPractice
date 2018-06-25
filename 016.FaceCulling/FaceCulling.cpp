#define STB_IMAGE_IMPLEMENTATION
#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <Minor/Shape/TextureCube.h>
#include <Minor/Shape/TexturePlane.h>

using namespace minor;
class FaceCulling : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "FaceCulling";
		_camera = ObserveCamera::create(_info.width, _info.height);
	}

	void startup()
	{
		Application::startup();

		_cube = TextureCube::create("marble.jpg");
		_plane = TexturePlane::create("metal.png");

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	}

	void render()
	{
		Application::render();

		_cube->setPosition(glm::vec3(-1.0f, 0.01f, -1.0f));
		_cube->drawShape();
		_cube->setPosition(glm::vec3(2.0f, 0.01f, 0.0f));
		_cube->drawShape();

		_plane->drawShape();
	}

	void shutdown()
	{
		Application::shutdown();
	}
private:
	TextureCube* _cube;
	TexturePlane* _plane;
};

DEBUG_MAIN(FaceCulling);