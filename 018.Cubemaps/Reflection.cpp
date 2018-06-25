#define STB_IMAGE_IMPLEMENTATION
#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <Minor/Skybox.h>
#include <vector>
#include <Minor/Shape/NormalCube.h>
#include <Minor/Model/Model.h>

using namespace minor;
class Reflection : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "Reflection";
		_camera = ObserveCamera::create(_info.width, _info.height);
	}

	void startup()
	{
		Application::startup();

		int num = 1;
		char* format = "jpg";
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

		string fsPath = _refraction ? "refraction.fs" : "reflection.fs";
		if (!_useModel)
		{
			_cube = NormalCube::create("reflection.vs", fsPath);
		}
		else
		{
			//_marisaShader = new Shader("modelDiffuse.vs", "modelDiffuse.fs");
			_modelShader = new Shader("reflection.vs", fsPath.c_str());
			//_model = new Model("Marisa/marisa.obj", false);
			_model = new Model("nanosuit/nanosuit.obj");
		}
	}

	void render()
	{
		Application::render();
		
		if (_cube != nullptr)
		{
			Shader* cubeShader = _cube->getShader();
			cubeShader->use();
			cubeShader->setVec3("cameraPos", _camera->getPosition());
			//_skybox->BindTexture();
			_cube->drawShape();
		}
		
		if (_model != nullptr)
		{
			_modelShader->use();
			_modelShader->setMat4("projection", _camera->getProjection());
			_modelShader->setMat4("view", _camera->getView());
			_modelShader->setVec3("cameraPos", _camera->getPosition());
			glm::mat4 model;
			model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
			_modelShader->setMat4("model", model);
			_model->Draw(_modelShader);
		}

		_skybox->Draw();
	}

	void shutdown()
	{
		Application::shutdown();
		_skybox->destory();
		if (_cube != nullptr) _cube->destory();
	}
private:
	SkyBox* _skybox;
	NormalCube* _cube;
	Model* _model;
	Shader* _modelShader;

	bool _useModel = true;
	bool _refraction = true;
};

DEBUG_MAIN(Reflection);