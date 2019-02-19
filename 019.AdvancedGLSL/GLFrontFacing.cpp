#define STB_IMAGE_IMPLEMENTATION
#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <Minor/Shape/TextureCube.h>

using namespace minor;
class GLFrontFacing : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "GLFrontFacing";
		_camera = ObserveCamera::create(_info.width, _info.height);
	}
	
	void startup()
	{
		char* vs = 
		{
			"#version 430 core												\n"
			"layout(location = 0) in vec3 pos;								\n"
			"layout(location = 1) in vec2 texCoord;							\n"
			"out vec2 TexCoord;												\n"
			"uniform mat4 model;											\n"
			"uniform mat4 view;												\n"
			"uniform mat4 projection;										\n"
			"void main()													\n"
			"{																\n"
			"	gl_Position = projection * view * model * vec4(pos, 1.0);	\n"
			"	TexCoord = texCoord;										\n"
			"}																\n"
		};

		char* fs = 
		{
			"#version 430 core									\n"
			"uniform sampler2D frontTex;						\n"
			"uniform sampler2D backTex;							\n"
			"out vec4 fragColor;								\n"
			"in vec2 TexCoord;									\n"
			"void main()										\n"
			"{													\n"
			"	if (gl_FrontFacing)								\n"
			"		fragColor = texture(frontTex, TexCoord);	\n"
			"	else											\n"
			"		fragColor = texture(backTex, TexCoord);		\n"
			"}													\n"
		};

		Application::startup();
		_cube = TextureCube::create();
		Shader* shader = new Shader(vs, fs, DATA);
		_cube->setShader(shader);

		_frontTexId = TextureLoader::Inst()->LoadTexture2D("marble.jpg");
		_backTexId = TextureLoader::Inst()->LoadTexture2D("wall.jpg");
		shader->use();
		shader->setInt("frontTexture", 0);
		shader->setInt("backTexture", 1);
		glActiveTexture(GL_TEXTURE0);
		glActiveTexture(GL_TEXTURE1);
	}
	
	void render()
	{
		Application::render();

		_cube->bindVertexArray();
		_cube->getShader()->use();
		_cube->getShader()->setInt("frontTex", 0);
		_cube->getShader()->setInt("backTex", 1);
		_cube->changeView();
		_cube->changeModel();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _frontTexId);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _backTexId);
		
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

	void shutdown()
	{
		Application::shutdown();
	}
private:
	TextureCube* _cube;
	unsigned int _backTexId;
	unsigned int _frontTexId;
};

DEBUG_MAIN(GLFrontFacing);