//#define STB_IMAGE_IMPLEMENTATION
#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <Minor/Model/Model.h>

using namespace minor;
class DecomposeObject : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "DecomposeObject";
		_info.backgroundColor = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);
		_camera = ObserveCamera::create(_info.width, _info.height);
	}

	void startup()
	{
		Application::startup();

		//_model = new Model("../Resources/Marisa/marisa.obj", false);
		_model = new Model("../Resources/nanosuit/nanosuit.obj");
		_shader = new Shader("../Libraries/Shader/modelDiffuse3.vs", "../Libraries/Shader/modelDiffuse.fs");
		_shader->LoadGeometryShader("../Libraries/Shader/decompose.gs");
	}

	void render()
	{
		Application::render();

		glm::mat4 model;
		_shader->use();
		_shader->setMat4("projection", _camera->getProjection());
		_shader->setMat4("view", _camera->getView());
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::translate(model, glm::vec3(0, -7.5f, -5.0f));
		_shader->setMat4("model", model);
		_shader->setFloat("time", curTime);

		_model->Draw(_shader);
	}

	void shutdown()
	{
		Application::shutdown();
	}
private:
	Model* _model;
	Shader* _shader;
};

//DEBUG_MAIN(DecomposeObject);