#ifndef __DrictionLightingMapMaterial__H__
#define __DrictionLightingMapMaterial__H__

#include <Minor/Shader.h>
#include <Minor/Shape/ShapeBase.h>
#include <Minor/Base/MComponent.h>

namespace minor
{
	class DrictionLightingMapMaterial : public MComponent
	{
	public:
		DrictionLightingMapMaterial()
		{
			name = "DrictionLightingMapMaterial";
			material.shininess = 32.0f;
			material.diffuse = 0;
			material.specular = 1;

			light.ambient = glm::vec3(0.1f);
			light.diffuse = glm::vec3(0.5f);
			light.specular = glm::vec3(1.0f);
			light.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
		}

		struct Material
		{
			unsigned int diffuse;
			unsigned int specular;
			float shininess;
		};
		
		struct Light
		{
			glm::vec3 direction;
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

			shader->setVec3("light.direction", light.direction);
			shader->setVec3("light.ambient", light.ambient);
			shader->setVec3("light.diffuse", light.diffuse);
			shader->setVec3("light.specular", light.specular);
		}

		Material material;
		Light light;
	};
	REGISTER(DrictionLightingMapMaterial);
};

#endif // __DrictionLightingMapMaterial__H__
