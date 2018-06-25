#define STB_IMAGE_IMPLEMENTATION
#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <Minor/Shape/TextureCube.h>
#include <Minor/Shape/TexturePlane.h>
#include <MInor/Shape/ColorCube.h>

using namespace minor;
class StencilTest : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "StencilTest";
		_camera = ObserveCamera::create(_info.width, _info.height);
	}
	
	void startup()
	{
		Application::startup();

		_cube = TextureCube::create("marble.jpg");
		_outlining = ColorCube::create(glm::vec3(0.04f, 0.28f, 0.26f), "normal.vs", "constant.fs");
		_plane = TexturePlane::create("metal.png");

		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	}
	
	void render()
	{
		Application::render();
		
		glClear(GL_STENCIL_BUFFER_BIT);

		glStencilMask(0x00);
		_plane->drawShape();

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		_cube->setPosition(glm::vec3(-1.0f, 0.01f, -1.0f));
		_cube->drawShape();
		_cube->setPosition(glm::vec3(2.0f, 0.01f, 0.0f));
		_cube->drawShape();

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		_outlining->setScale(glm::vec3(1.05f));
		_outlining->setPosition(glm::vec3(-1.0f, 0.01f, -1.0f));
		_outlining->drawShape();
		_outlining->setPosition(glm::vec3(2.0f, 0.01f, 0.0f));
		_outlining->drawShape();

		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);
	}

	void shutdown()
	{
		Application::shutdown();
	}
private:
	ColorCube* _outlining;
	TextureCube* _cube;
	TexturePlane* _plane;
};

DEBUG_MAIN(StencilTest);