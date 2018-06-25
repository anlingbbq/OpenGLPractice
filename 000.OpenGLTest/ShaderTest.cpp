#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <Minor/Shape/ColorCube.h>

using namespace minor;
class ShaderTest : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "ShaderTest";
		_camera = ObserveCamera::create(_info.width, _info.height);
	}
	
	void startup()
	{
		Application::startup();
		_cube = ColorCube::create(glm::vec3(0), "../Libraries/Shader/normal.vs", "testMat.fs");
	}
	
	void render()
	{
		Application::render();
		_cube->getShader()->use();
		_cube->getShader()->setMat3("testMat", testMat);
		_cube->drawShape();
	}

	void shutdown()
	{
		Application::shutdown();
	}
private:
	ColorCube* _cube;
	Shader* _shader;

	glm::mat3 testMat = glm::mat3(0, 1, 0, 0, 0, 0, 0, 0, 0);
};

//DEBUG_MAIN(ShaderTest);