//#define STB_IMAGE_IMPLEMENTATION
#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <Minor/Model/Model.h>

using namespace minor;
class AsteroidFieldWithoutInstance : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "AsteroidField";
		_camera = ObserveCamera::create(_info.width, _info.height);
		_camera->setMoveSpeed(6);
		_camera->setZfar(1000);
	}
	
	void startup()
	{
		Application::startup();

		_planet = new Model("planet/planet.obj");
		_rock = new Model("rock/rock.obj");

		_shader = new Shader("modelDiffuse.vs", "modelDiffuse.fs");

		float radius = 50.0f;
		float offset = 5.0f;
		int amount = 3000;
		for (int i = 0; i < amount; i++)
		{
			glm::mat4 model;
			float angle = (float)i / (float)amount * 360.0f;
			float displacement = Rangef(-offset, offset);
			float x = sin(angle) * radius + displacement;
			displacement = Rangef(-offset, offset);
			float y = displacement * 0.4f;
			displacement = Rangef(-offset, offset);
			float z = cos(angle) * radius + displacement;
			model = glm::translate(model, glm::vec3(x, y, z));

			float scale = Rangef(0.05f, 0.25f);
			model = glm::scale(model, glm::vec3(scale));

			float rotAangle = Randomf() * 360.0f;
			model = glm::rotate(model, rotAangle, glm::vec3(0.4f, 0.6f, 0.8f));

			_rockMatrices.push_back(model);
		}

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
	}
	
	void render()
	{
		Application::render();

		_shader->use();
		_shader->setMat4("projection", _camera->getProjection());
		_shader->setMat4("view", _camera->getView());

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		_shader->setMat4("model", model);
		_planet->Draw(_shader);

		int amount = _rockMatrices.size();
		for (int i = 0; i < amount; i++)
		{
			_shader->setMat4("model", _rockMatrices[i]);
			_rock->Draw(_shader);
		}
	}

	void shutdown()
	{
		Application::shutdown();
	}
private:
	Model* _planet;
	Model* _rock;

	Shader* _shader;
	std::vector<glm::mat4> _rockMatrices;
};

//DEBUG_MAIN(AsteroidFieldWithoutInstance);