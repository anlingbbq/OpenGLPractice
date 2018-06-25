#define STB_IMAGE_IMPLEMENTATION
#include <vector>
#include <map>
#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <Minor/Shape/TextureCube.h>
#include <Minor/Shape/TexturePlane.h>
#include <Minor/Shape/TextureQuad.h>

#define BlendTest true

static char* grass_fs = {
	"#version 430 core									\n"
	"uniform sampler2D ourTexture;						\n"
	"out vec4 fragColor;								\n"
	"in vec2 TexCoord;									\n"
	"void main()										\n"
	"{													\n"
	"	vec4 texColor = texture(ourTexture, TexCoord);	\n"
	"	if (texColor.a < 0.1) 							\n"
	"		discard; 									\n"
	"	fragColor = texColor;							\n"
	"}													\n"
};

using namespace minor;
class BlendingTest : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "BlendingTest";
		_camera = ObserveCamera::create(_info.width, _info.height);
	}

	void startup()
	{
		Application::startup();

		_objPos.push_back(glm::vec3(-1.0f, 0.0f, -0.48f));
		_objPos.push_back(glm::vec3( 2.0f, 0.0f,  0.51f));
		_objPos.push_back(glm::vec3( 0.5f, 0.0f,  0.7f));
		_objPos.push_back(glm::vec3( 0.2f, 0.0f, -2.3f));
		_objPos.push_back(glm::vec3( 1.0f, 0.0f, -0.6f));

#if BlendTest == false
		_obj = TextureQuad::create("grass.png", grass_fs);
		
#else
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		_obj = TextureQuad::create("transparent_window.png");
#endif

		_cube = TextureCube::create("marble.jpg");
		_plane = TexturePlane::create("metal.png");

		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_FRONT);
	}

	void render()
	{
		Application::render();

		_cube->setPosition(glm::vec3(-1.0f, 0.01f, -1.0f));
		_cube->drawShape();
		_cube->setPosition(glm::vec3(2.0f, 0.01f, 0.0f));
		_cube->drawShape();

		_plane->drawShape();

#if BlendTest == false
		for (unsigned int i = 0; i < _objPos.size(); i++)
		{
			_obj->setPosition(_objPos[i]);
			_obj->drawShape();
		}
#else
		// the transparent parts are written to the depth buffer as any other value,
		// so the depth test will discards them
		//for (unsigned int i = 0; i < _objPos.size(); i++)
		//{
		//	_obj->setPosition(_objPos[i]);
		//	_obj->drawShape();
		//}

		// sort the transparent windows before rendering
		std::map<float, glm::vec3> sorted;
		for (unsigned int i = 0; i < _objPos.size(); i++)
		{
			float distance = glm::length(_camera->getPosition() - _objPos[i]);
			sorted[distance] = _objPos[i];
		}

		// draw the farthest object first and the closest object as last
		std::map<float, glm::vec3>::reverse_iterator iter = sorted.rbegin();
		for (; iter != sorted.rend(); iter++)
		{
			_obj->setPosition(iter->second);
			_obj->drawShape();
		}
#endif
	}

	void shutdown()
	{
		Application::shutdown();
	}
private:
	TextureQuad* _obj;
	TextureCube* _cube;
	TexturePlane* _plane;

	std::vector<glm::vec3> _objPos;
};

DEBUG_MAIN(BlendingTest);