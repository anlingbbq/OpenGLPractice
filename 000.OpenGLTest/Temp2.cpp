#define STB_IMAGE_IMPLEMENTATION

#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <iostream>
#include <thread>

constexpr int getConst() {
	return 2;
}
int arr[getConst()] = { 1, 2 };

template<typename ...T> void DummyWrapper(T... t) {}

template<typename T> T pr(T t)
{
	std::cout << t;
	return t;
}

template<typename ...A>
void MyPrint(A... a)
{
	DummyWrapper(pr(a)...);
}

void thread_task()
{
	 std::cout << "hello thread" << std::endl;
}

using namespace minor;
class Temp2 : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "Temp2";
		_camera = ObserveCamera::create(_info.width, _info.height);

		glfwWindowHint(GLFW_SAMPLES, 4);
	}
	
	void thread_task()
	{
		std::cout << "hello thread" << std::endl;
	}

	void startup()
	{
		Application::startup();

		//std::thread t(thread_task);
		//t.join();
	}

	void render()
	{
		Application::render();
	}

	void shutdown()
	{
		Application::shutdown();
	}
private:
};

//DEBUG_MAIN(Temp2);
