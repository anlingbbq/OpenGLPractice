#ifndef __MINOR_H__
#define __MINOR_H__

#include <Windows.h>
#include <iostream>
#include <ctime>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// MATH
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "CameraBase.h"
#include "TextureLoader.h"
#include "Shader.h"

namespace minor
{
	class Application
	{
	public:
		Application() {}
		virtual ~Application() {}

		virtual void run(minor::Application* this_app)
		{
			bool running = true;
			app = this_app;

			if (!glfwInit())
			{
				printf("Faild to initialize GLEW\n");
				return;
			}

			init();

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _info.majorVersion);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _info.minorVersion);

			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

			_window = glfwCreateWindow(_info.width, _info.height, _info.title.c_str(), _info.flags.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
			if (!_window)
			{
				printf("Failed to create GLFW window\n");
				glfwTerminate();
				system("pause");
				return;
			}
			
			glfwMakeContextCurrent(_window);

			glewExperimental = GL_TRUE;
			glewInit();

			glfwSetWindowSizeCallback(_window, glfw_onResize);
			glfwSetKeyCallback(_window, glfw_onKey);
			glfwSetMouseButtonCallback(_window, glfw_onMouseButton);
			glfwSetCursorPosCallback(_window, glfw_onMouseMove);
			glfwSetScrollCallback(_window, glfw_onMouseWheel);

			if (!_info.flags.cursor)
			{
				glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			}

			printf("Starting GLFW Context, OPenGL %d.%d\n", _info.majorVersion, _info.minorVersion);

			startup();

			do
			{
				curTime = (float)glfwGetTime();
				deltaTime = curTime - lastTime;

				glfwSwapBuffers(_window);
				glfwPollEvents();

				if (_camera != nullptr)
					_camera->update(deltaTime);

				render();

				if (_camera != nullptr)
					_camera->setChangeTrans(false);

				lastTime = curTime;

				running &= (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_RELEASE);
				running &= (glfwWindowShouldClose(_window) != GL_TRUE);

			} while (running);

			shutdown();

			glfwDestroyWindow(_window);
			glfwTerminate();
		}

		virtual void init()
		{
			_info.title = "minor";
			_info.width = 800;
			_info.height = 600;

			_info.majorVersion = 4;
			_info.minorVersion = 3;

			_info.samples = 0;
			_info.flags.all = 0;
			_info.flags.cursor = 1;

			_info.backgroundColor = glm::vec4(0);
			_camera = nullptr;

			srand((unsigned)time(NULL));
		}

		virtual void startup()
		{
			glEnable(GL_DEPTH_TEST);
		}

		virtual void render()
		{
			glClearColor(_info.backgroundColor.r, _info.backgroundColor.g, _info.backgroundColor.b, _info.backgroundColor.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		virtual void shutdown()
		{
			if (_camera != nullptr)
			{
				delete _camera;
				_camera = nullptr;
			}
		}

		void setWindowTitle(const char * title)
		{
			glfwSetWindowTitle(_window, title);
		}

		virtual void onResize(int w, int h)
		{
			_info.width = w;
			_info.height = h;
		}

		virtual void onKey(int key, int action)
		{
			if (_camera != nullptr)
				_camera->onKey(key, action);
		}

		virtual void onMouseButton(int button, int action)
		{
			if (_camera != nullptr)
				_camera->onMouseButton(button, action);
		}

		virtual void onMouseMove(int posX, int posY)
		{
			if (_camera != nullptr)
				_camera->onMouseMove(posX, posY);
		}

		virtual void onMouseWheel(int posX, int posY)
		{
			if (_camera != nullptr)
				_camera->onMouseWheel(posX, posY);
		}

		void getMousePosition(int& x, int& y)
		{
			double dx, dy;
			glfwGetCursorPos(_window, &dx, &dy);

			x = static_cast<int>(floor(dx));
			y = static_cast<int>(floor(dy));
		}

		CameraBase* getCamera()
		{
			return _camera;
		}

		glm::mat4 getCameraView()
		{
			if (_camera != nullptr)
				return _camera->getView();

			return glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), 
				glm::vec3(0.0f, 0.0f, 3.0f) + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));;
		}

		glm::mat4 getCameraProjection()
		{
			if (_camera != nullptr)
				return _camera->getProjection();

			return glm::perspective(glm::radians(45.0f), (float)_info.width / _info.height, 0.1f, 100.0f);
		}

		bool getCameraChangeTrans()
		{
			//if (_camera != nullptr)
				//return _camera->getChangeTrans();

			return true;
		}

	public:
		struct APPINFO
		{
			std::string title;
			int width;
			int height;
			int majorVersion;
			int minorVersion;
			int samples;
			glm::vec4 backgroundColor;
			union
			{
				struct
				{
					unsigned int    fullscreen : 1;
					unsigned int    vsync : 1;
					unsigned int    cursor : 1;
					unsigned int    stereo : 1;
					unsigned int    debug : 1;
					unsigned int    robust : 1;
				};
				unsigned int all;
			} flags;
		};

		static minor::Application* app;

		GLfloat curTime		= 0.0f;
		GLfloat deltaTime	= 0.0f;
		GLfloat lastTime	= 0.0f;

		glm::vec2 getWinSize()
		{
			return glm::vec2(_info.width, _info.height);
		}

	protected:
		APPINFO _info;
		GLFWwindow* _window;
		CameraBase* _camera;

		static void glfw_onResize(GLFWwindow* window, int w, int h)
		{
			app->onResize(w, h);
		}

		static void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			app->onKey(key, action);
		}

		static void glfw_onMouseButton(GLFWwindow* window, int button, int action, int mods)
		{
			app->onMouseButton(button, action);
		}

		static void glfw_onMouseMove(GLFWwindow* window, double posx, double posy)
		{
			app->onMouseMove(static_cast<int>(posx), static_cast<int>(posy));
		}

		static void glfw_onMouseWheel(GLFWwindow* window, double xoffset, double yoffset)
		{
			app->onMouseWheel(static_cast<int>(xoffset), static_cast<int>(yoffset));
		}

		void setVsync(bool enable)
		{
			_info.flags.vsync = enable ? 1 : 0;
			glfwSwapInterval((int)_info.flags.vsync);
		}
	};
};


#define DECLARE_MAIN(a)                             \
minor::Application* Application::app;				\
minor::TextureLoader* TextureLoader::inst;			\
int CALLBACK WinMain(HINSTANCE hInstance,           \
					 HINSTANCE hPrevInstance,       \
					 LPSTR lpCmdLine,               \
					 int nCmdShow)                  \
{                                                   \
	a *app = new a;                                 \
	app->run(app);                                  \
	delete app;                                     \
	return 0;                                       \
}													\

#define DEBUG_MAIN(a)								\
minor::Application* Application::app;				\
minor::TextureLoader* TextureLoader::inst;			\
int main()											\
{													\
	a *app = new a;									\
	app->run(app);									\
	delete app;										\
	return 0;										\
}													\

// return int random[a, b]
#define Range(a,b) (rand()%(b-a+1)+a)

// return float random(0, 1)
#define Randomf() (float)(rand()/double(RAND_MAX))
// return float random(a, b)
#define Rangef(a, b) (float)((rand() / double(RAND_MAX) * (b - a) + a));


#endif // !__SHADER_H__