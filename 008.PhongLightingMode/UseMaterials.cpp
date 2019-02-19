#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <Minor/Shape/ColorCube.h>
#include <Minor/Materials/PhongMaterial.h>

using namespace minor;
class UseMaterials : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "UseMaterials";
		_info.backgroundColor = glm::vec4(0, 0, 0, 1);
		_camera = ObserveCamera::create(_info.width, _info.height);
	}

	void startup()
	{
		Application::startup();

		_lamp = ColorCube::create(glm::vec3(1, 1, 1), "normal.vs", "constant.fs");
		_lamp->setPosition(_lampPos);
		_lamp->setScale(glm::vec3(0.2f));

		_cube = ColorCube::create(glm::vec3(1.0f, 0.5f, 0.31f), "normal.vs", "phongMaterial.fs");
		_cube->getShader()->setVec3("lightColor", _lamp->getColor());

		//_cube->getShader()->setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
		//_cube->getShader()->setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
		//_cube->getShader()->setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		//_cube->getShader()->setFloat("material.shininess", 32.0f);

		// 测试组件方式
		//PhongMaterial* mat = new PhongMaterial();
		//_cube->AddComponent(mat);
		//_cube->AddComponent("PhongMaterial");
		_cube->AddComponent<PhongMaterial>();

		//PhongMaterial* mat = (PhongMaterial*)_cube->GetComponent("PhongMaterial");
		PhongMaterial* mat = _cube->GetComponent<PhongMaterial>();
		if (mat != nullptr) mat->useMaterial();
	}

	void render()
	{
		Application::render();

		_lamp->drawShape();
		_cube->drawShape();

		_cube->getShader()->setVec3("viewPos", _camera->getPosition());
		_cube->getShader()->setVec3("lightPos", _lampPos);
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

	const glm::vec3 _lampPos = glm::vec3(1.2f, 1.0f, 2.0f);
};

DEBUG_MAIN(UseMaterials);