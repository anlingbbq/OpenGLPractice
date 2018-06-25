#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <Minor/Shape/ColorCube.h>

using namespace minor;
class Specular : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "Specular";
		_info.backgroundColor = glm::vec4(0, 0, 0, 1);
		_camera = ObserveCamera::create(_info.width, _info.height);

		glfwWindowHint(GLFW_SAMPLES, 4);
	}

	void startup()
	{
		Application::startup();

		_lamp = ColorCube::create(glm::vec3(1, 1, 1), "normal.vs", "constant.fs");
		_lamp->setPosition(_lampPos);
		_lamp->setScale(glm::vec3(0.2f));

		_cube = ColorCube::create(glm::vec3(1.0f, 0.5f, 0.31f), "normal.vs", "specular.fs");
		_cube->getShader()->setVec3("lightColor", _lamp->getColor());
	}

	void render()
	{
		Application::render();

		_lamp->drawShape();
		_cube->drawShape();

		_cube->getShader()->setVec3("viewPos", _camera->getPosition());

		glm::vec3 lampPos = glm::vec3(glm::cos(curTime) * _radius, 1.0f, glm::sin(curTime) * _radius);
		_lamp->setPosition(lampPos);
		//glm::vec3 lampPos = _lampPos;
		_cube->getShader()->setVec3("lightPos", lampPos);
	}

	void shutdown()
	{
		Application::shutdown();

		_lamp->destory();
		_cube->destory();
	}
private:
	ColorCube* _lamp;
	ColorCube* _cube;

	const float _radius = 2.0f;
	const glm::vec3 _lampPos = glm::vec3(1.2f, 1.0f, 2.0f);
};

DEBUG_MAIN(Specular);