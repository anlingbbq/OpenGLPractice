#ifndef __LightingMapMaterial__H__
#define __LightingMapMaterial__H__

#include <Minor/Shader.h>
#include <Minor/Shape/ShapeBase.h>
#include <Minor/Base/MComponent.h>

namespace minor
{
	class LightingMapMaterial : public MComponent
	{
	public:
		LightingMapMaterial()
		{
			name = "LightingMapMaterial";
			material.shininess = 32.0f;
			material.diffuse = 0;
			material.specular = 1;

			light.ambient = glm::vec3(0.2f);
			light.diffuse = glm::vec3(0.5f);
			light.specular = glm::vec3(1.0f);
			light.position = glm::vec3(0);
		}

		struct Material
		{
			unsigned int diffuse;
			unsigned int specular;
			float shininess;
		};
		
		struct Light
		{
			glm::vec3 position;
			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
		};

		void useMaterial()
		{
			Shader* shader = ((ShapeBase*)gameObject)->getShader();
			shader->use();
			shader->setInt("material.diffuse", material.diffuse);
			shader->setInt("material.specular", material.specular);
			shader->setFloat("material.shininess", material.shininess);

			shader->setVec3("light.position", light.position);
			shader->setVec3("light.ambient", light.ambient);
			shader->setVec3("light.diffuse", light.diffuse);
			shader->setVec3("light.specular", light.specular);
		}

		Material material;
		Light light;
	};
	REGISTER(LightingMapMaterial);
};

#endif // __LightingMapMaterial__H__
