#ifndef _SkyBox_H_
#define _SkyBox_H_

#include "Base/MComponent.h"
#include "Shader.h"
#include <vector>
#include "TextureLoader.h"
#include "Minor.h"

namespace minor
{
	static float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	class SkyBox : public MComponent
	{
	public:
		SkyBox(std::vector<std::string> faces)
		{
			unsigned int skyboxVBO;
			glGenVertexArrays(1, &_skyboxVAO);
			glGenBuffers(1, &skyboxVBO);
			glBindVertexArray(_skyboxVAO);
			glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

			_textureID = TextureLoader::Inst()->LoadCubemap(faces);
			//_textureID = TextureManager::Inst()->LoadCubemap(faces);

			_shader = new Shader("skybox.vs", "skybox.fs");
			_shader->use();
			_shader->setInt("skybox", 0);
		};

		static SkyBox* create(int index = 1)
		{
			std::vector<std::string> faces
			{
				"skybox_" + std::to_string(index) + "/right.jpg",
				"skybox_" + std::to_string(index) + "/left.jpg",
				"skybox_" + std::to_string(index) + "/top.jpg",
				"skybox_" + std::to_string(index) + "/bottom.jpg",
				"skybox_" + std::to_string(index) + "/front.jpg",
				"skybox_" + std::to_string(index) + "/back.jpg",
			};
			return SkyBox::create(faces);
		}

		static SkyBox* create(std::vector<std::string> faces)
		{
			SkyBox* skyBox = new SkyBox(faces);
			return skyBox;
		};

		void Draw()
		{
			_shader->use();
			// set view and projection matrix
			if (Application::app->getCameraChangeTrans())
			{
				_shader->setMat4("view", glm::mat3(Application::app->getCameraView()));
				_shader->setMat4("projection", Application::app->getCameraProjection());
			}

			glDepthFunc(GL_LEQUAL);

			glBindVertexArray(_skyboxVAO);
			glBindTexture(GL_TEXTURE_CUBE_MAP, _textureID);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			glDepthFunc(GL_LESS);
		}

		void BindTexture()
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, _textureID);
		}

	private:
		unsigned int _skyboxVAO;
		unsigned int _textureID;
		Shader* _shader;
	};
}

#endif // !_SkyBox_H_
