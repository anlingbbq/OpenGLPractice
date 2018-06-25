#define STB_IMAGE_IMPLEMENTATION
#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <Minor/Shape/TextureCube.h>
#include <Minor/Shape/TexturePlane.h>

using namespace minor;
class DepthTest : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "DepthTest";
		_camera = ObserveCamera::create(_info.width, _info.height);
	}
	
	void startup()
	{
		Application::startup();
		_cube = TextureCube::create("marble.jpg");
		_plane = TexturePlane::create("metal.png");

		//glDepthFunc(GL_ALWAYS);
	}
	
	void render()
	{
		Application::render();

		glDisable(GL_POLYGON_OFFSET_FILL);

		_cube->setPosition(glm::vec3(-1.0f, 0.0f, -1.0f));
		_cube->drawShape();

		// prevent z-fightting
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0f, 0.0f);

		_cube->setPosition(glm::vec3(2.0f, 0.0f, 0.0f));
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

DEBUG_MAIN(DepthTest);