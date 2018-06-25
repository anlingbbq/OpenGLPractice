//#define STB_IMAGE_IMPLEMENTATION
#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <Minor/Skybox.h>
#include <vector>
#include <Minor/Shape/TextureCube.h>

using namespace minor;
class Cubemaps : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "Cubemaps";
		_info.width = 800;
		_info.height = 600;
		_camera = ObserveCamera::create(_info.width, _info.height);
	}
	
	void startup()
	{
		Application::startup();

		int num = 2;
		char* format = "bmp";
		std::vector<std::string> faces
		{
			"skybox_" + std::to_string(num) + "/right." + format,
			"skybox_" + std::to_string(num) + "/left." + format,
			"skybox_" + std::to_string(num) + "/top." + format,
			"skybox_" + std::to_string(num) + "/bottom." + format,
			"skybox_" + std::to_string(num) + "/front." + format,
			"skybox_" + std::to_string(num) + "/back." + format,
		};
		_skybox = SkyBox::create(faces);
		_cube = TextureCube::create("box.png");

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	
	void render()
	{
		Application::render();
		_cube->drawShape();
		_skybox->Draw();
	}

	void shutdown()
	{
		Application::shutdown();
		_cube->destory();
		_skybox->destory();
	}
private:
	SkyBox* _skybox;
	TextureCube* _cube;
};

//DEBUG_MAIN(Cubemaps);