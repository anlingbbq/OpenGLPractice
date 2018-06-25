#ifndef __PhongMaterial__H__
#define __PhongMaterial__H__

#include <Minor/Base/MComponent.h>

namespace minor
{
	class PhongMaterial : public MComponent
	{
	public:
		PhongMaterial()
		{
			name = "PhongMaterial";
			material.ambint = glm::vec3(1.0f, 0.5f, 0.31f);
			material.diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
			material.specular = glm::vec3(0.5f, 0.5f, 0.5f);
			material.shininess = 32.0f;

			light.ambient = glm::vec3(0.2f);
			light.diffuse = glm::vec3(0.5f);
			light.specular = glm::vec3(1.0f);
		}

		struct Material
		{
			float shininess;
			glm::vec3 ambint;
			glm::vec3 diffuse;
			glm::vec3 specular;
		};
		
		struct Light
		{
			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
		};

		void useMaterial()
		{
			Shader* shader = ((ShapeBase*)gameObject)->getShader();
			shader->setVec3("material.ambient", material.ambint);
			shader->setVec3("material.diffuse", material.diffuse);
			shader->setVec3("material.specular", material.specular);
			shader->setFloat("material.shininess", material.shininess);

			shader->setVec3("light.ambient", light.ambient);
			shader->setVec3("light.diffuse", light.diffuse);
			shader->setVec3("light.specular", light.specular);
		}

		Material material;
		Light light;
	};
	REGISTER(PhongMaterial);
};

#endif // __PhongMaterial__H__
