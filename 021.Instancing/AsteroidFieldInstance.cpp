#define STB_IMAGE_IMPLEMENTATION
#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <Minor/Model/Model.h>

using namespace minor;
class AsteroidFieldInstance : public Application
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
		_instanceShader = new Shader("modelDiffuseInstance.vs", "modelDiffuse.fs");

		float radius = 150.0f;
		float offset = 25.0f;
		for (int i = 0; i < amount; i++)
		{
			glm::mat4 model;
			float angle = (float)i / (float)amount * 360.0f;
			float displacement = Rangef(-offset, offset);
			float x = sin(angle) * radius + displacement;
			displacement = Rangef(-offset, offset);
			float y = displacement * 0.2f;
			displacement = Rangef(-offset * 2, offset * 2);
			float z = cos(angle) * radius + displacement;
			model = glm::translate(model, glm::vec3(x, y, z));

			float scale = Rangef(0.05f, 0.25f);
			model = glm::scale(model, glm::vec3(scale));

			float rotAangle = Randomf() * 360.0f;
			model = glm::rotate(model, rotAangle, glm::vec3(0.4f, 0.6f, 0.8f));

			_rockMatrices[i] = model;
		}

		unsigned int buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &_rockMatrices[0], GL_STATIC_DRAW);

		for (unsigned int i = 0; i < _rock->meshes.size(); i++)
		{
			unsigned int vao = _rock->meshes[i].vao;
			glBindVertexArray(vao);

			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);

			glBindVertexArray(0);
		}

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
	}

	void render()
	{
		Application::render();

		glm::mat4 projection = _camera->getProjection();
		glm::mat4 view = _camera->getView();

		_instanceShader->use();
		_instanceShader->setMat4("projection", projection);
		_instanceShader->setMat4("view", view);
		_shader->use();
		_shader->setMat4("projection", projection);
		_shader->setMat4("view", view);

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		_shader->setMat4("model", model);
		_planet->Draw(_shader);

		_instanceShader->use();
		_instanceShader->setInt("material.texture_diffuse1", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _rock->texture_loaded[0].id);
		for (unsigned int i = 0; i < _rock->meshes.size(); i++)
		{
			glBindVertexArray(_rock->meshes[i].vao);
			glDrawElementsInstanced(GL_TRIANGLES, _rock->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
			glBindVertexArray(0);
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
	Shader* _instanceShader;

	static const int amount = 100000;
	glm::mat4 _rockMatrices[amount];
};

DEBUG_MAIN(AsteroidFieldInstance);