#define STB_IMAGE_IMPLEMENTATION
#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <Minor/Shape/TextureCube.h>
#include <Minor/Shape/TexturePlane.h>
#include <Minor/Materials/Kernel3x3Material.h>

#define UseKernel false

using namespace minor;
class Framebuffers : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "Framebuffers";
		_camera = ObserveCamera::create(_info.width, _info.height);
	}

	void startup()
	{
		Application::startup();

#if UseKernel == false
		// no effect scene shader
		//_sceneShader = new Shader("quad.vs", "quad.fs");

		// inversion color effect
		_sceneShader = new Shader("quad.vs", "inversion.fs");

		// grayscale color effect
		//_sceneShader = new Shader("quad.vs", "grayscale.fs");
#else
		// kernel effect
		_sceneShader = new Shader("quad.vs", "kernel3x3.fs");
		_mat = new Kernel3x3Material();

		// choose effect
		//_mat->useSharpenEffect();
		_mat->useBlurEffect();
		//_mat->useEdgeDetection();

		/*float f = 20;
		float d = 20;
		float c = 1;
		float t = f * 4 + d * 4 + c;
		_mat->kernel = glm::mat3
		(
			f / t, d / t, f / t,
			d / t, c / t, d / t,
			f / t, d / t, f / t
		);*/
#endif
		_sceneShader->use();
		_sceneShader->setInt("screenTexture", 0);

		// bind vertices use for framebuffer
		glGenVertexArrays(1, &_quadVao);
		glGenBuffers(1, &_quadVbo);
		glBindVertexArray(_quadVao);

		glBindBuffer(GL_ARRAY_BUFFER, _quadVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_quadVertices), &_quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		// framebuffer configuration
		glGenFramebuffers(1, &_framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
		// create a color attachment texture
		glGenTextures(1, &_textureColorbuffer);
		glBindTexture(GL_TEXTURE_2D, _textureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _info.width, _info.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _textureColorbuffer, 0);
		// create a renderbuffer object for depth and stencil attachment
		unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _info.width, _info.height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
		// check it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			printf("framebuffer error : framebuffer is not complete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		_cube = TextureCube::create("wall.jpg");
		_plane = TexturePlane::create("metal.png");
	}

	void render()
	{
		if (_useEffect)
			effectRender();
		else
			normalRender();
	}

	void normalRender()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glEnable(GL_DEPTH_TEST);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		_cube->setPosition(glm::vec3(-1.0f, 0.01f, -1.0f));
		_cube->drawShape();
		_cube->setPosition(glm::vec3(2.0f, 0.01f, 0.0f));
		_cube->drawShape();
		_plane->drawShape();
	}

	void effectRender()
	{
		// bind to framebuffer and draw scene as we normally would to color texture
		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
		glEnable(GL_DEPTH_TEST);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		_cube->setPosition(glm::vec3(-1.0f, 0.01f, -1.0f));
		_cube->drawShape();
		_cube->setPosition(glm::vec3(2.0f, 0.01f, 0.0f));
		_cube->drawShape();
		_plane->drawShape();

		// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// disable depth test so screen-space quad isn't discard due to depth test
		glDisable(GL_DEPTH_TEST);
		// clear all relevant buffers, it's not really necessary
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		_sceneShader->use();
#if UseKernel == true
		_mat->useMaterial(_sceneShader);
#endif

		glBindVertexArray(_quadVao);
		// use the color attachment	texture as the texture of the quad plane
		glBindTexture(GL_TEXTURE_2D, _textureColorbuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void shutdown()
	{
		Application::shutdown();
	}

	void onKey(int key, int action) override
	{
		Application::onKey(key, action);
		if (key == GLFW_KEY_F)
		{
			if (action == GLFW_PRESS)
				_useEffect = true;
			else if (action == GLFW_RELEASE)
				_useEffect = false;
		}
	}

private:
	TextureCube* _cube;
	TexturePlane* _plane;

	unsigned int _quadVao, _quadVbo;

	unsigned int _framebuffer;
	unsigned int _textureColorbuffer;

	Shader* _sceneShader;
	Kernel3x3Material* _mat;
	float _quadVertices[24] =
	{
		 // positions  // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	bool _useEffect = true;
};

DEBUG_MAIN(Framebuffers);