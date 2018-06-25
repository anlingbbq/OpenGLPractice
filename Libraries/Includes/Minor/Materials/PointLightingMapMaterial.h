#ifndef __PointLightingMapMaterial__H__
#define __PointLightingMapMaterial__H__

#include <vector>
#include <Minor/Shader.h>
#include <Minor/Shape/ShapeBase.h>
#include <Minor/Base/MComponent.h>

namespace minor
{
	class PointLightingMapMaterial : public MComponent
	{
	public:
		PointLightingMapMaterial(int num = 1)
		{
			name = "PointLightingMapMaterial";
			material.shininess = 32.0f;
			material.diffuse = 0;
			material.specular = 1;

			pointNum = num;
			for (unsigned int i = 0; i < pointNum; i++)
			{
				Point light;
				light.ambient = glm::vec3(0.2f);
				light.diffuse = glm::vec3(0.5f);
				light.specular = glm::vec3(1.0f);
				light.position = glm::vec3(0);

				light.constant = 1.0f;
				light.linear = 0.09f;
				light.quadratic = 0.032f;

				points.push_back(light);
			}
		}

		struct Material
		{
			unsigned int diffuse;
			unsigned int specular;
			float shininess;
		};
		
		struct Point
		{
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
			useMaterial(shader);
		}

		void useMaterial(Shader* shader)
		{
			shader->use();
			shader->setInt("material.diffuse", material.diffuse);
			shader->setInt("material.specular", material.specular);
			shader->setFloat("material.shininess", material.shininess);

			for (unsigned i = 0; i < pointNum; i++)
			{
				std::string index = std::to_string(i);
				shader->setVec3("pointLights[" + index + "].position", points[i].position);
				shader->setVec3("pointLights[" + index + "].ambient", points[i].ambient);
				shader->setVec3("pointLights[" + index + "].diffuse", points[i].diffuse);
				shader->setVec3("pointLights[" + index + "].specular", points[i].specular);
				shader->setFloat("pointLights[" + index + "].constant", points[i].constant);
				shader->setFloat("pointLights[" + index + "].linear", points[i].linear);
				shader->setFloat("pointLights[" + index + "].quadratic", points[i].quadratic);
			}
		}

		Material material;
		std::vector<Point> points;
		unsigned int pointNum = 1;
	};
	REGISTER(PointLightingMapMaterial);
};

#endif // __PointLightingMapMaterial__H__
