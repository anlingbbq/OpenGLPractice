#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <Minor/Shape/ColorCube.h>

using namespace minor;
class Diffuse : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "Diffuse";
		_camera = ObserveCamera::create(_info.width, _info.height);
	}

	void startup()
	{
		Application::startup();

		_lamp = ColorCube::create(glm::vec3(1, 1, 1), "normal.vs", "constant.fs");
		_lamp->setPosition(_lampPos);
		_lamp->setScale(glm::vec3(0.2f));

		_cube = ColorCube::create(glm::vec3(1.0f, 0.5f, 0.31f), "normal.vs", "diffuse.fs");
		_cube->getShader()->setVec3("lightColor", _lamp->getColor());
		_cube->getShader()->setVec3("lightPos", _lampPos);
		
	}

	void render()
	{
		Application::render();

		_lamp->drawShape();
		_cube->drawShape();
	}

	void shutdown()
	{
		_lamp->destory();
		_cube->destory();
	}
private:
	ColorCube* _lamp;
	ColorCube* _cube;

	const glm::vec3 _lampPos = glm::vec3(1.2f, 1.0f, 2.0f);
};

//DEBUG_MAIN(Diffuse);