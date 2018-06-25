#ifndef __MutipleLightingMaterial__H__
#define __MutipleLightingMaterial__H__

#include <Minor/Shader.h>
#include <Minor/Shape/ShapeBase.h>
#include <Minor/Base/MComponent.h>

namespace minor
{
	class MutipleLightingMaterial : public MComponent
	{
	public:
		MutipleLightingMaterial()
		{
			name = "MutipleLightingMaterial";
			material.shininess = 32.0f;
			material.diffuse = 0;
			material.specular = 1;

			dirLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
			dirLight.ambient = glm::vec3(0.05f);
			dirLight.diffuse = glm::vec3(0.4f);
			dirLight.specular = glm::vec3(0.5f);

			for (int i = 0; i < pointNum; i++)
			{
				pointLights[i].position = glm::vec3(0);
				pointLights[i].ambient = glm::vec3(0.05f);
				pointLights[i].diffuse = glm::vec3(0.8f);
				pointLights[i].specular = glm::vec3(1.0f);
				pointLights[i].constant = 1.0f;
				pointLights[i].linear = 0.09f;
				pointLights[i].quadratic = 0.032f;
			}

			spotLight.position = glm::vec3(0);
			spotLight.direction = glm::vec3(0);
			spotLight.ambient = glm::vec3(0);
			spotLight.diffuse = glm::vec3(1.0f);
			spotLight.specular = glm::vec3(1.0f);
			spotLight.constant = 1.0f;
			spotLight.linear = 0.09f;
			spotLight.quadratic = 0.032f;
			spotLight.cutOff = glm::cos(glm::radians(12.5f));
			spotLight.outerCutOff = glm::cos(glm::radians(15.0f));
		}

		struct Material
		{
			unsigned int diffuse;
			unsigned int specular;
			float shininess;
		};
		
		struct DirLight
		{
			glm::vec3 direction;
			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
		};

		struct PointLight
		{
			glm::vec3 position;
			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
			float constant;
			float linear;
			float quadratic;
		};

		struct SpotLight
		{
			glm::vec3 position;
			glm::vec3 direction;
			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
			float constant;
			float linear;
			float quadratic;
			float cutOff;
			float outerCutOff;
		};

		void useMaterial()
		{
			Shader* shader = ((ShapeBase*)gameObject)->getShader();
			shader->use();
			shader->setInt("material.diffuse", material.diffuse);
			shader->setInt("material.specular", material.specular);
			shader->setFloat("material.shininess", material.shininess);

			shader->setVec3("dirLight.direction", dirLight.direction);
			shader->setVec3("dirLight.ambient", dirLight.ambient);
			shader->setVec3("dirLight.diffuse", dirLight.diffuse);
			shader->setVec3("dirLight.specular", dirLight.specular);

			for (int i = 0; i < pointNum; i++)
			{
				std::string index = std::to_string(i);
				shader->setVec3("pointLights[" + index + "].position", pointLights[i].position);
				shader->setVec3("pointLights[" + index + "].ambient", pointLights[i].ambient);
				shader->setVec3("pointLights[" + index + "].diffuse", pointLights[i].diffuse);
				shader->setVec3("pointLights[" + index + "].specular", pointLights[i].specular);
				shader->setFloat("pointLights[" + index + "].constant", pointLights[i].constant);
				shader->setFloat("pointLights[" + index + "].linear", pointLights[i].linear);
				shader->setFloat("pointLights[" + index + "].quadratic", pointLights[i].quadratic);
			}

			shader->setVec3("spotLight.position", spotLight.position);
			shader->setVec3("spotLight.direction", spotLight.direction);
			shader->setVec3("spotLight.ambient", spotLight.ambient);
			shader->setVec3("spotLight.diffuse", spotLight.diffuse);
			shader->setVec3("spotLight.specular", spotLight.specular);
			shader->setFloat("spotLight.constant", spotLight.constant);
			shader->setFloat("spotLight.linear", spotLight.linear);
			shader->setFloat("spotLight.quadratic", spotLight.quadratic);
			shader->setFloat("spotLight.cutOff", spotLight.cutOff);
			shader->setFloat("spotLight.outerCutOff", spotLight.outerCutOff);
		}

		void OpenSpot()
		{
			spotLight.ambient = glm::vec3(0);
			spotLight.diffuse = glm::vec3(1.0f);
			spotLight.specular = glm::vec3(1.0f);
		}

		void CloseSpot()
		{
			spotLight.ambient = glm::vec3(0);
			spotLight.diffuse = glm::vec3(0);
			spotLight.specular = glm::vec3(0);
		}

		Material material;
		DirLight dirLight;
		static const int pointNum = 4;
		PointLight point1;
		PointLight point2;
		PointLight point3;
		PointLight point4;
		PointLight pointLights[pointNum] = { point1, point2, point3, point4 };
		SpotLight spotLight;
	};
	REGISTER(MutipleLightingMaterial);
};

#endif // __MutipleLightingMaterial__H__
