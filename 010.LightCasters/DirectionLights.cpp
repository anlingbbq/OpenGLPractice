#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <Minor/Shape/LightingMapCube.h>
#include <Minor/Materials/DirctionLightingMapMaterial.h>

using namespace minor;
class DirectionLights : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "DirectionLights";
		_info.backgroundColor = glm::vec4(0, 0, 0, 1);
		_camera = ObserveCamera::create(_info.width, _info.height);
	}
	
	void startup()
	{
		Application::startup();

		_cube = LightingMapCube::create("box.png", "box_map.png", "dirctionLightMap.fs");
		_cube->AddComponent<DrictionLightingMapMaterial>()->useMaterial();
	}
	
	void render()
	{
		Application::render();

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
	}
private:
	LightingMapCube* _cube;

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
};

//DEBUG_MAIN(DirectionLights);
