//#define STB_IMAGE_IMPLEMENTATION
#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <Minor/Model/Model.h>

using namespace minor;
class VisualizingNormalVectors : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "VisualizingNormalVectors";
		_camera = ObserveCamera::create(_info.width, _info.height);
	}
	
	void startup()
	{
		Application::startup();
		
		_modelShader = new Shader("modelDiffuse.vs", "modelDiffuse.fs");
		_normalShader = new Shader("normalVisualization.vs", "color.fs");
		_normalShader->LoadGeometryShader("normalVisualization.gs");

		//_model = new Model("nanosuit/nanosuit.obj");
		_model = new Model("Marisa/marisa.obj", false);
	}
	
	void render()
	{
		Application::render();

		glm::mat4 model;
		_modelShader->use();
		_modelShader->setMat4("projection", _camera->getProjection());
		_modelShader->setMat4("view", _camera->getView());
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		//model = glm::translate(model, glm::vec3(0, -7.5f, -5.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
		_modelShader->setMat4("model", model);
		_model->Draw(_modelShader);

		_normalShader->use();
		_normalShader->setMat4("projection", _camera->getProjection());
		_normalShader->setMat4("view", _camera->getView());
		_normalShader->setMat4("model", model);
		_model->Draw(_normalShader);
	}

	void shutdown()
	{
		Application::shutdown();
	}
private:
	Shader* _modelShader;
	Shader* _normalShader;
	Model* _model;
};

//DEBUG_MAIN(VisualizingNormalVectors);