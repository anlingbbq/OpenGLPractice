#include <Minor/Minor.h>

using namespace minor;
class CreatWindow : public Application
{
	void init()
	{
		Application::init();
		_info.title = "CreatWindow";
	}

	void startup()
	{
	}

	void render()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void shutdown()
	{
	}
};

DEBUG_MAIN(CreatWindow);