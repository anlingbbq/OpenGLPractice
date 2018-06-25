#define STB_IMAGE_IMPLEMENTATION
#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <Minor/Model/Model.h>
#include <Minor/Shader.h>
#include <Minor/Materials/PointLightingMapMaterial.h>

#define UsePointLight false

using namespace minor;
class LoadModel : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "LoadModel";
		_camera = ObserveCamera::create(_info.width, _info.height);
		_info.backgroundColor = glm::vec4(0);
	}
	
	void startup()
	{
		Application::startup();

#if UsePointLight == false
		_shader = new Shader("modelDiffuse.vs", "modelDiffuse.fs");
#else
		_shader = new Shader("common.vs", "pointLightMap2.fs");
		_mat = new PointLightingMapMaterial(2);
		for (unsigned int i = 0; i < _mat->points.size(); i++)
		{
			_mat->points[0].position = pointLightPositions[i];
			_mat->points[0].ambient = glm::vec3(0.5f);
			_mat->points[0].diffuse = glm::vec3(1);
			_mat->points[0].specular = glm::vec3(1);
			_mat->points[0].linear = 0.009f;
			_mat->points[0].quadratic = 0.0032f;
		}
#endif
		_marisa = new Model("Marisa/marisa.obj", false);
		//_nanosuit = new Model("nanosuit/nanosuit.obj");
	}
	
	void render()
	{
		Application::render();
		_shader->use();

#if UsePointLight == true
		_mat->useMaterial(_shader);
		_shader->setVec3("viewPos", _camera->getPosition());
#endif
		glm::mat4 model;
		_shader->setMat4("projection", _camera->getProjection());
		_shader->setMat4("view", _camera->getView());
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		if (_nanosuit != nullptr)
		{
			model = glm::translate(model, glm::vec3(0, -1.75f, 0));
			_shader->setMat4("model", model);
			_nanosuit->Draw(_shader);
		}
		
		if (_marisa != nullptr)
		{
			model = glm::translate(model, glm::vec3(8, 0, 0));
			model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
			_shader->setMat4("model", model);
			_marisa->Draw(_shader);
		}
	}

	void shutdown()
	{
		Application::shutdown();
	}
private:
	Model* _nanosuit;
	Model* _marisa;
	Shader* _shader;
	PointLightingMapMaterial* _mat;

	glm::vec3 pointLightPositions[2] = {
		glm::vec3(2.3f, -1.6f, -3.0f),
		glm::vec3(-1.7f, 0.9f, 1.0f)
	};
};

DEBUG_MAIN(LoadModel);