//#define STB_IMAGE_IMPLEMENTATION
#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/Shape/ColorCube.h>
#include <Minor/Shape/TextureCube.h>

using namespace minor;
class Transform3D : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "Transform3D";
	}
	
	void startup()
	{
		_texCube = TextureCube::create("wall.jpg");

		_texCube->setView(glm::vec3(0, 0, -3.0f));
		_texCube->setProjection(45, (float)_info.width / _info.height, 0.1f, 100.0f);

		glEnable(GL_DEPTH_TEST);
	}
	
	void render()
	{
		Application::render();
		_texCube->setRotate(curTime * 20.0f, glm::vec3(0.5f, 1.0f, 0.0f));
		_texCube->drawShape();
	}

	void shutdown()
	{
		_texCube->clear();
	}
private:
	TextureCube* _texCube;
};

DEBUG_MAIN(Transform3D)