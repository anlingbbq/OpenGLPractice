#define STB_IMAGE_IMPLEMENTATION

#include <Minor/Minor.h>
#include <Minor/ObserveCamera.h>

using namespace minor;
class HDR : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "HDR";
		_camera = ObserveCamera::create(_info.width, _info.height);

		glfwWindowHint(GLFW_SAMPLES, 4);
	}
	
	void startup()
	{
		Application::startup();

		_lightingShader = new Shader("hdrLighting.vs", "hdrLighting.fs");
		_hdrShader = new Shader("hdr.vs", "hdr.fs");
	}
	
	void render()
	{
		Application::render();
	}

	void shutdown()
	{
		Application::shutdown();
	}

private:
	Shader* _lightingShader;
	Shader* _hdrShader;
};

DEBUG_MAIN(HDR);