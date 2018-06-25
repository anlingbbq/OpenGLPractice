#define STB_IMAGE_IMPLEMENTATION
#include <Minor/Minor.h>
#include <Minor/Shape/TextureCube.h>
#include <Minor/ObserveCamera.h>

using namespace minor;
class UseCamera : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "UseCamera";
		
		_camera = ObserveCamera::create(_info.width, _info.height);
	}
	
	void startup()
	{
		Application::startup();

		_texCube = TextureCube::create("wall.jpg");

		//_texCube->setView(glm::vec3(0, 0, -3.0f));
		//_texCube->setProjection(45, (float)_info.width / _info.height, 0.1f, 100.0f);
	}
	
	void render()
	{
		Application::render();

		for (size_t i = 0; i < 10; i++)
		{
			_texCube->setPosition(_cubePositions[i]);
			_texCube->setRotate(20.0f * i, glm::vec3(1.0f, 0.3f, 0.5f));
			_texCube->drawShape();
		}
		//printf("\n%f", this->getCamera()->getPosition().z);
	}

	void shutdown()
	{
		Application::shutdown();

		_texCube->clear();
	}
private:
	TextureCube* _texCube;

	glm::vec3 _cubePositions[10] = {
		glm::vec3( 0.0f,  0.0f,  0.0f),
		glm::vec3( 2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3( 2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3( 1.3f, -2.0f, -2.5f),
		glm::vec3( 1.5f,  2.0f, -2.5f),
		glm::vec3( 1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
};

DEBUG_MAIN(UseCamera);