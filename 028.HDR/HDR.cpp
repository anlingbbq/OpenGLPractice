#define STB_IMAGE_IMPLEMENTATION

#include <Minor/Minor.h>
#include <Minor/ObserveCamera.h>

using namespace minor;
class HDR : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "HDR";
		_camera = ObserveCamera::create(_info.width, _info.height);

		glfwWindowHint(GLFW_SAMPLES, 4);
	}
	
	void startup()
	{
		Application::startup();

		_lightingShader = new Shader("hdrLighting.vs", "hdrLighting.fs");
		_hdrShader = new Shader("hdr.vs", "hdr.fs");

		_woodTexture = TextureLoader::Inst()->LoadTexture2D("wood.png", true);

		// configure floating point framebuffer
		glGenFramebuffers(1, &_hdrFBO);
		// create floating point color buffer
		// ------------------------------------
		glGenTextures(1, &_colorBuffer);
		glBindTexture(GL_TEXTURE_2D, _colorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _info.width, _info.height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// create depth buffer (renderbuffer)
		unsigned int rboDepth;
		glGenRenderbuffers(1, &rboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _info.width, _info.height);
		// attach buffers
		glBindFramebuffer(GL_FRAMEBUFFER, _hdrFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorBuffer, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// lighting info
		// -------------
		// positions
		_lightPositions.push_back(glm::vec3(0.0f, 0.0f, 49.5f)); // back light
		_lightPositions.push_back(glm::vec3(-1.4f, -1.9f, 9.0f));
		_lightPositions.push_back(glm::vec3(0.0f, -1.8f, 4.0f));
		_lightPositions.push_back(glm::vec3(0.8f, -1.7f, 6.0f));
		// colors
		_lightColors.push_back(glm::vec3(200.0f, 200.0f, 200.0f));
		_lightColors.push_back(glm::vec3(0.1f, 0.0f, 0.0f));
		_lightColors.push_back(glm::vec3(0.0f, 0.0f, 0.2f));
		_lightColors.push_back(glm::vec3(0.0f, 0.1f, 0.0f));


		// shader configuration
		// --------------------
		_lightingShader->use();
		_lightingShader->setInt("diffuseTexture", 0);
		_hdrShader->use();
		_hdrShader->setInt("hdrBuffer", 0);
	}
	
	void render()
	{
		Application::render();

		// 1. render scene into floating point framebuffer
		// -----------------------------------------------
		glBindFramebuffer(GL_FRAMEBUFFER, _hdrFBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 projection = _camera->getProjection();
		glm::mat4 view = _camera->getView();
		_lightingShader->use();
		_lightingShader->setMat4("projection", projection);
		_lightingShader->setMat4("view", view);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _woodTexture);
		// set lighting uniforms
		for (unsigned int i = 0; i < _lightPositions.size(); i++)
		{
			_lightingShader->setVec3("lights[" + std::to_string(i) + "].Position", _lightPositions[i]);
			_lightingShader->setVec3("lights[" + std::to_string(i) + "].Color", _lightColors[i]);
		}
		_lightingShader->setVec3("viewPos", _camera->getPosition());
		// render tunnel
		glm::mat4 model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 27.5f));
		_lightingShader->setMat4("model", model);
		_lightingShader->setInt("inverse_normals", true);
		renderCube();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 2. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
		// --------------------------------------------------------------------------------------------------------------------------
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		_hdrShader->use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _colorBuffer);
		_hdrShader->setInt("hdr", _hdr);
		_hdrShader->setFloat("exposure", _exposure);
		renderQuad();
	}

	void shutdown()
	{
		Application::shutdown();
	}

	void renderCube()
	{
		// initialize (if necessary)
		if (_cubeVAO == 0)
		{
			float vertices[] = {
				// back face
				-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
				1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
				1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
				1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
				-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
				-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
				// front face
				-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
				1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
				1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
				1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
				-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
				-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
				// left face
				-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
				-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
				-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
				-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
				-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
				-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
				// right face
				1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
				1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
				1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
				1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
				1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
				1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
				// bottom face
				-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
				1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
				1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
				1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
				-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
				-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
				// top face
				-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
				1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
				1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
				1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
				-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
				-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
			};
			glGenVertexArrays(1, &_cubeVAO);
			glGenBuffers(1, &_cubeVBO);
			// fill buffer
			glBindBuffer(GL_ARRAY_BUFFER, _cubeVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			// link vertex attributes
			glBindVertexArray(_cubeVAO);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
		// render Cube
		glBindVertexArray(_cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

	void renderQuad()
	{
		if (_quadVAO == 0)
		{
			float quadVertices[] = {
				// positions        // texture Coords
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};
			// setup plane VAO
			glGenVertexArrays(1, &_quadVAO);
			glGenBuffers(1, &_quadVBO);
			glBindVertexArray(_quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		glBindVertexArray(_quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	void onKey(int key, int action) override
	{
		Application::onKey(key, action);
		if (key == GLFW_KEY_F)
		{
			if (action == GLFW_PRESS)
				_hdr = true;
			else if (action == GLFW_RELEASE)
				_hdr = false;
		}

		if (key == GLFW_KEY_O)
		{
			_exposure += 0.1f;
		}
		if (key == GLFW_KEY_L)
		{
			_exposure -= 0.1f;
		}
	}

private:
	bool _hdr = true;
	float _exposure = 1.0f;

	Shader* _lightingShader;
	Shader* _hdrShader;

	unsigned int _hdrFBO;
	unsigned int _woodTexture;
	unsigned int _colorBuffer;

	std::vector<glm::vec3> _lightPositions;
	std::vector<glm::vec3> _lightColors;

	unsigned int _cubeVAO = 0;
	unsigned int _cubeVBO = 0;
	unsigned int _quadVAO = 0;
	unsigned int _quadVBO = 0;
};

DEBUG_MAIN(HDR);