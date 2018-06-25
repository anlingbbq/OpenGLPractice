//#define STB_IMAGE_IMPLEMENTATION

#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <functional>

#define PRINT_STR(x) printf("%s\n", #x)

#define ERROR(...) {								\
	printf("ERROR::\n");							\
	LOG(__VA_ARGS__);								\
}													\

#define LOG(...) {									\
	printf("%s: Line %d\t", __FILE__, __LINE__);	\
	printf(__VA_ARGS__);							\
	printf("\n");									\
}													\

#define LOG(...) printf("...")

using namespace std;
class test1
{
public:
	test1() {};
	test1(string name)  {
		_name = name;
		printf("name:%s\n", name.c_str());
	};
	string _name;
};

class test2 : public test1
{
public:
	using test1::test1;
	test2()
	{
	}
};
//test2* b = new test2(); error

using namespace minor;
class Temp1 : public Application
{
public:
	std::function<void()> funcTest;
	void func2Test()
	{
		printf(">>>>>");
	}

	void init()
	{
		Application::init();
		_info.title = "Temp1";
		_camera = ObserveCamera::create(_info.width, _info.height);

		glfwWindowHint(GLFW_SAMPLES, 4);
	}

	void startup()
	{
		Application::startup();
		
		string str = "test";
		funcTest = [str]() 
		{ 
			printf(str.c_str()); 
		};
		funcTest();

		//printf("\n%d", sizeof(bool)); // 1

		//PRINT_STR(2);
		//LOG("x = %d", 2);
		//ERROR("x = 2");
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

//DEBUG_MAIN(Temp1);