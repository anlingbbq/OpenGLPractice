#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <Minor/Shape/ColorCube.h>
#include <Minor/Shape/TextureCube.h>

using namespace minor;
class OffscreenMSAA : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "OffScreenMSAA";
		_camera = ObserveCamera::create(_info.width, _info.height);
	}

	void startup()
	{
		Application::startup();
		
		// Cube
		_cube = ColorCube::create(glm::vec3(0, 1, 0), "normal.vs", "constant.fs");
		glm::quat r1 = _cube->setRotate(45, glm::vec3(1, 0, 0));
		glm::quat r2 = _cube->setRotate(45, glm::vec3(0, 1, 0));
		_cube->setRotate(r1, r2);

		// FrameBuffer-Quad
		_screenShader = new Shader("quad.vs", "quad.fs");
		glGenVertexArrays(1, &_quadVAO);
		glGenBuffers(1, &_quadVBO);
		glBindVertexArray(_quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_quadVertices), &_quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		// FrameBuffer
		glGenFramebuffers(1, &_framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
		// create a multisampled color attachment texture
		unsigned int textureColorBufferMultiSampled;
		glGenTextures(1, &textureColorBufferMultiSampled);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, _info.width, _info.height, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);
		// create a (also multisampled) renderbuffer object for depth and stencil attachments
		unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, _info.width, _info.height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// configure second post-processing framebuffer
		glGenFramebuffers(1, &_intermediateFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, _intermediateFBO);
		// create a color attachment texture
		glGenTextures(1, &_screenTexture);
		glBindTexture(GL_TEXTURE_2D, _screenTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _info.width, _info.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _screenTexture, 0);	// we only need a color buffer

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// shader configuration
		// --------------------
		_screenShader->setInt("screenTexture", 0);
	}

	void render()
	{
		Application::render();

		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		
		_cube->drawShape();

		// blit multisampled buffer(s) to normal colorbuffer of intermediate FBO.Image is stored in screenTexture
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _intermediateFBO);
		glBlitFramebuffer(0, 0, _info.width, _info.height, 0, 0, _info.width, _info.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		// render quad with scene's visuals as its texture image
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);

		// draw Screen quad
		_screenShader->use();
		glBindVertexArray(_quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _screenTexture); // use the now resolved color attachment as the quad's texture
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void shutdown()
	{
		Application::shutdown();
	}
private:
	ColorCube* _cube;
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
	unsigned int _quadVAO, _quadVBO;
	Shader* _screenShader;
	unsigned int _framebuffer;
	unsigned int _intermediateFBO;
	unsigned int _screenTexture;
};

DEBUG_MAIN(OffscreenMSAA);