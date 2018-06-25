#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <Minor/Shape/ColorCube.h>
#include <Minor/Shape/LightingMapCube.h>
#include <Minor/Materials/MutipleLightingMaterial.h>

using namespace minor;
class MultipleLights : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "MultipleLights";
		_info.backgroundColor = glm::vec4(0);
		_camera = ObserveCamera::create(_info.width, _info.height);
	}

	void startup()
	{
		Application::startup();
		_lamp = ColorCube::create(glm::vec3(1), "normal.vs", "constant.fs");
		_lamp->setScale(glm::vec3(0.2f));
		_cube = LightingMapCube::create("box.png", "box_map.png", "multipleLight.fs");
		_cube->AddComponent<MutipleLightingMaterial>();
	}

	void render()
	{
		Application::render();

		_lamp->drawShape();
		for (int i = 0; i <= _pointLightPositions->length(); i++)
		{
			_lamp->setPosition(_pointLightPositions[i]);
			_lamp->drawShape();
		}

		MutipleLightingMaterial* mat = _cube->GetComponent<MutipleLightingMaterial>();
		for (size_t i = 0; i < mat->pointNum; i++)
		{
			mat->pointLights[i].position = _pointLightPositions[i];
		}
		if (_openSpot)
		{
			mat->spotLight.position = _camera->getPosition();
			mat->spotLight.direction = _camera->getFront();
			mat->OpenSpot();
		}
		else
		{
			mat->CloseSpot();
		}

		mat->useMaterial();
		_cube->getShader()->setVec3("viewPos", _camera->getPosition());

		for (int i = 0; i < 10; i++)
		{
			_cube->setPosition(_cubePositions[i]);
			_cube->setRotate(20.0f * i, glm::vec3(1.0f, 0.3f, 0.5f));
			_cube->drawShape();
		}
	}

	void shutdown()
	{
		Application::shutdown();
		_cube->destory();
		_lamp->destory();
	}

	virtual void onKey(int key, int action)
	{
		Application::onKey(key, action);
		if (action == GLFW_PRESS)
		{
			if (key == GLFW_KEY_F)
			{
				_openSpot = !_openSpot;
			}
		}
	}

private:
	ColorCube* _lamp;
	LightingMapCube* _cube;

	glm::vec3 _pointLightPositions[4] = {
		glm::vec3( 0.7f,  0.2f,  2.0f),
		glm::vec3( 2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3( 0.0f,  0.0f, -3.0f)
	};

	glm::vec3 _cubePositions[10] = {
		glm::vec3( 0.0f,  0.0f,  0.0f),
		glm::vec3( 2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3( 2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3( 1.3f, -2.0f, -2.5f),
		glm::vec3( 1.5f,  2.0f, -2.5f),
		glm::vec3( 1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	bool _openSpot = true;
};

DEBUG_MAIN(MultipleLights);