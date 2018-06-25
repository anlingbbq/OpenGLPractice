#define STB_IMAGE_IMPLEMENTATION

#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <iostream>

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
	
	void startup()
	{
		Application::startup();

		//printf("arr num %d", arr[0]);
		MyPrint("1, ", "2, ", "3");
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

DEBUG_MAIN(Temp2);