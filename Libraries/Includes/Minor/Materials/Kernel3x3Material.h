#ifndef __PhongMaterial__H__
#define __PhongMaterial__H__

#include <Minor/Shader.h>
#include <Minor/Minor.h>
#include <Minor/Shape/ShapeBase.h>
#include <Minor/Base/MComponent.h>

namespace minor
{
	class Kernel3x3Material : public MComponent
	{
	public:
		float offset;
		glm::mat3 kernel;

		Kernel3x3Material()
		{
			name = "Kernel3x3Material";
			glm::vec2 size = Application::app->getWinSize();
			offset = 2.0f / (size.x > size.y ? size.y : size.x);
		}

		void useMaterial(Shader* shader = NULL)
		{
			if (shader == NULL)
				shader = ((ShapeBase*)gameObject)->getShader();
			if (shader == NULL)
				return;

			shader->setFloat("offset", offset);
			shader->setMat3("kernel", kernel);
		}

		// sharpen effect
		void useSharpenEffect()
		{
			kernel = glm::mat3
			(
				-1, -1, -1,
				-1, 9, -1,
				-1, -1, -1
			);
		}

		float blurScale = 1.0f;
		void useBlurEffect()
		{
			kernel = glm::mat3
			(
				1.0 / 16, 2.0 / 16, 1.0 / 16,
				2.0 / 16, 4.0 / 16, 2.0 / 16,
				1.0 / 16, 2.0 / 16, 1.0 / 16
			);
		}

		void useEdgeDetection()
		{
			kernel = glm::mat3
			(
				1, 1, 1,
				1,-8, 1,
				1, 1, 1
			);
		}
	};
	REGISTER(Kernel3x3Material);
};

#endif // __PhongMaterial__H__
