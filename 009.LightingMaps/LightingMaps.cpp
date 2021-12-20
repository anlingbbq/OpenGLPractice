#define STB_IMAGE_IMPLEMENTATION
#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <Minor/Shape/ColorCube.h>
#include <Minor/Shape/LightingMapCube.h>
#include <Minor/Materials/LightingMapMaterial.h>

using namespace minor;
class LightingMaps : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "LightingMaps";
		_info.backgroundColor = glm::vec4(0, 0, 0, 1);
		_camera = ObserveCamera::create(_info.width, _info.height);
	}

	void startup()
	{
		Application::startup();

		_lamp = ColorCube::create(glm::vec3(1, 1, 1), "normal.vs", "constant.fs");
		_lamp->setPosition(_lampPos);
		_lamp->setScale(glm::vec3(0.2f));

		_cube = LightingMapCube::create("box.png", "box_map.png");
		LightingMapMaterial* mat = _cube->AddComponent<LightingMapMaterial>();
		if (mat != nullptr) {
			mat->light.position = _lampPos;
			mat->useMaterial();
		}
	}

	void render()
	{
		Application::render();

		_lamp->drawShape();
		_cube->drawShape();

		_cube->getShader()->setVec3("viewPos", _camera->getPosition());
	}

	void shutdown()
	{
		Application::shutdown();

		_lamp->destory();
		_cube->destory();
	}
private:
	ColorCube* _lamp;
	LightingMapCube* _cube;

	const glm::vec3 _lampPos = glm::vec3(1.2f, 1.0f, 2.0f);
};

DEBUG_MAIN(LightingMaps);