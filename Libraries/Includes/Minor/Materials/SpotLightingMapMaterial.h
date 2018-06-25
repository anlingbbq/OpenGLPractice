#ifndef __SpotLightingMapMaterial__H__
#define __SpotLightingMapMaterial__H__

#include <Minor/Shader.h>
#include <Minor/Shape/ShapeBase.h>
#include <Minor/Base/MComponent.h>

namespace minor
{
	class SpotLightingMapMaterial : public MComponent
	{
	public:
		SpotLightingMapMaterial()
		{
			name = "SpotLightingMapMaterial";
			material.shininess = 32.0f;
			material.diffuse = 0;
			material.specular = 1;

			light.ambient = glm::vec3(0.2f);
			light.diffuse = glm::vec3(0.8f);
			light.specular = glm::vec3(1.0f);
			light.position = glm::vec3(0);
			light.direction = glm::vec3(0);

			light.constant = 1.0f;
			light.linear = 0.09f;
			light.quadratic = 0.032f;

			light.cutOff = glm::cos(glm::radians(12.5f));
			light.outerCutOff = glm::cos(glm::radians(17.5f));
		}

		struct Material
		{
			unsigned int diffuse;
			unsigned int specular;
			float shininess;
		};
		
		struct Light
		{
			float cutOff;
			float outerCutOff;
			glm::vec3 direction;
			glm::vec3 position;
			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;

			float constant;
			float linear;
			float quadratic;
		};

		void useMaterial()
		{
			Shader* shader = ((ShapeBase*)gameObject)->getShader();
			shader->use();
			shader->setInt("material.diffuse", material.diffuse);
			shader->setInt("material.specular", material.specular);
			shader->setFloat("material.shininess", material.shininess);

			shader->setFloat("light.cutOff", light.cutOff);
			shader->setFloat("light.outerCutOff", light.outerCutOff);
			shader->setVec3("light.direction", light.direction);
			shader->setVec3("light.position", light.position);
			shader->setVec3("light.ambient", light.ambient);
			shader->setVec3("light.diffuse", light.diffuse);
			shader->setVec3("light.specular", light.specular);
			shader->setFloat("light.constant", light.constant);
			shader->setFloat("light.linear", light.linear);
			shader->setFloat("light.quadratic", light.quadratic);
		}

		Material material;
		Light light;
	};
	REGISTER(SpotLightingMapMaterial);
};

#endif // __SpotLightingMapMaterial__H__
