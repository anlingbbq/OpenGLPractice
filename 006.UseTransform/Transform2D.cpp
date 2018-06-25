#define STB_IMAGE_IMPLEMENTATION
#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/Shape/ColorRectangle.h>

using namespace minor;
class Transform2D : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "UseTransform";
	}
	
	void startup()
	{
		_rect = ColorRectangle::create(glm::vec3(0.0f, 0.5f, 0.5f));
		_rect->setRotate(-55.0f, glm::vec3(1, 0, 0));

		_rect->setView(glm::vec3(0, 0, -3.0f));
		_rect->setProjection(45, (float)_info.width / _info.height, 0.1f, 100.0f);
	}
	
	void render()
	{
		_rect->addRotate(curTime * 50.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		_rect->drawShape();
	}

	void shutdown()
	{
		_rect->clear();
	}
private:
	ColorRectangle* _rect;
};

DEBUG_MAIN(Transform2D);