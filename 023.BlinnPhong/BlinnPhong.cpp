#define STB_IMAGE_IMPLEMENTATION
#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <Minor/Shape/TexturePlane.h>

using namespace minor;
class BlinnPhong : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "BlinnPhong";
		_camera = ObserveCamera::create(_info.width, _info.height);
	}
	
	void startup()
	{
		Application::startup();
		//_plane = TexturePlane::create("blinnPhong.fs", "wood.png");
		_plane = TexturePlane::create("compareBlinnWithPhong.fs", "wood.png");
	}
	
	void render()
	{
		Application::render();
		Shader* shader = _plane->getShader();
		shader->setVec3("viewPos", _camera->getPosition());
		shader->setVec3("lightPos", glm::vec3(0, 0, 0));
		_plane->drawShape();
	}

	void shutdown()
	{
		Application::shutdown();
	}
private:

	TexturePlane* _plane;
};

DEBUG_MAIN(BlinnPhong);