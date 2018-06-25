#include <Minor/Minor.h>
#include <Minor/Shape/ColorRectangle.h>

using namespace minor;
class Test : public Application
{
	void init()
	{
		Application::init();
		_info.title = "Test";
	}

	void startup()
	{
		_rect = ColorRectangle::create(glm::vec4(0, 1, 1, 1));
		_rect->setView(glm::vec3(0, 0, -3.0f));
		_rect->setProjection(45, (float)_info.width / _info.height, 0.1f, 100.0f);
		_rect->setRotate(45, glm::vec3(0, 0, 1));

		std::cout << typeid(ColorRectangle).name() << std::endl;
	}

	void render()
	{
		_rect->drawShape();
	}

	void shutdown()
	{
		_rect->clear();
	}

private:
	ColorRectangle* _rect;
};

//DEBUG_MAIN(Test);