#define STB_IMAGE_IMPLEMENTATION

#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/Model/Model.h>
#include <Minor/ObserveCamera.h>

using namespace minor;
class DeferredRender01 : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "DeferredRender01";
		_camera = ObserveCamera::create(_info.width, _info.height);

		glfwWindowHint(GLFW_SAMPLES, 4);
	}

	void startup()
	{
		Application::startup();

		_shaderGemotryPass = new Shader("gBuffer.vs", "gBuffer.fs");
		_shaderLightingPass = new Shader("deferredShading.vs", "deferredShading.fs");
		_shaderLightBox = new Shader("defferedLightBox.vs", "defferedLightBox.fs");

		// load models
		nanosuit = new Model("nanosuit/nanosuit.obj");
		_objectPositions.push_back(glm::vec3(-3.0, -3.0, -3.0));
		_objectPositions.push_back(glm::vec3( 0.0, -3.0, -3.0));
		_objectPositions.push_back(glm::vec3( 3.0, -3.0, -3.0));
		_objectPositions.push_back(glm::vec3(-3.0, -3.0, 0.0));
		_objectPositions.push_back(glm::vec3( 0.0, -3.0, 0.0));
		_objectPositions.push_back(glm::vec3( 3.0, -3.0, 0.0));
		_objectPositions.push_back(glm::vec3(-3.0, -3.0, 3.0));
		_objectPositions.push_back(glm::vec3( 0.0, -3.0, 3.0));
		_objectPositions.push_back(glm::vec3( 3.0, -3.0, 3.0));

		// configure g-buffer framebuffer
		glGenFramebuffers(1, &_gBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, _gBuffer);
		// position color buffer
		glGenTextures(1, &gPosition);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _info.width, _info.height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
		// normal color buffer
		glGenTextures(1, &gNormal);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _info.width, _info.height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
		// color + specular color buffer
		glGenTextures(1, &gAlbedoSpec);
		glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _info.width, _info.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
		// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
		unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);
		// create and attach depth buffer (renderbuffer)
		unsigned int rboDepth;
		glGenRenderbuffers(1, &rboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _info.width, _info.height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
		// finally check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// lighting info
		const unsigned int NR_LIGHTS = 32;

		srand(13);
		for (unsigned int i = 0; i < NR_LIGHTS; i++)
		{
			// calculate slightly random offsets
			float xPos = ((rand() % 100) / 100.0f) * 6.0f - 3.0f;
			float yPos = ((rand() % 100) / 100.0f) * 6.0f - 4.0f;
			float zPos = ((rand() % 100) / 100.0f) * 6.0f - 3.0f;
			_lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
			// also calculate random color
			float rColor = ((rand() % 100) / 200.0f) + 0.5f; // between 0.5 and 1.0
			float gColor = ((rand() % 100) / 200.0f) + 0.5f; // between 0.5 and 1.0
			float bColor = ((rand() % 100) / 200.0f) + 0.5f; // between 0.5 and 1.0
			_lightColors.push_back(glm::vec3(rColor, gColor, bColor));
		}

		// shader configuration
		// --------------------
		_shaderLightingPass->use();
		_shaderLightingPass->setInt("gPosition", 0);
		_shaderLightingPass->setInt("gNormal", 1);
		_shaderLightingPass->setInt("gAlbedoSpec", 2);
	}

	void render()
	{
		Application::render();

		// 1. geometry pass: render scene's geometry/color data into gbuffer
		glBindFramebuffer(GL_FRAMEBUFFER, _gBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 projection = _camera->getProjection();
		glm::mat4 view = _camera->getView();
		glm::mat4 model;
		_shaderGemotryPass->use();
		_shaderGemotryPass->setMat4("projection", projection);
		_shaderGemotryPass->setMat4("view", view);
		for (unsigned int i = 0; i < _objectPositions.size(); i++)
		{
			model = glm::mat4();
			model = glm::translate(model, _objectPositions[i]);
			model = glm::scale(model, glm::vec3(0.25f));
			_shaderGemotryPass->setMat4("model", model);
			nanosuit->Draw(_shaderGemotryPass);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 2. lighting pass: calculate lighting by iterating over a screen filled quad pixel-by-pixel using the gbuffer's content.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		_shaderLightingPass->use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
		// send light relevant uniforms
		for (unsigned int i = 0; i < _lightPositions.size(); i++)
		{
			_shaderLightingPass->setVec3("lights[" + std::to_string(i) + "].Position", _lightPositions[i]);
			_shaderLightingPass->setVec3("lights[" + std::to_string(i) + "].Color", _lightColors[i]);
			// update attenuation parameters and calculate radius
			const float constant = 1.0f; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
			const float linear = 0.7f;
			const float quadratic = 1.8f;
			_shaderLightingPass->setFloat("lights[" + std::to_string(i) + "].Linear", linear);
			_shaderLightingPass->setFloat("lights[" + std::to_string(i) + "].Quadratic", quadratic);
		}
		_shaderLightingPass->setVec3("viewPos", _camera->getPosition());
		// finally render quad
		renderQuad();

		// 2.5. copy content of geometry's depth buffer to default framebuffer's depth buffer
		// ----------------------------------------------------------------------------------
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _gBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
		glBlitFramebuffer(0, 0, _info.width, _info.height, 0, 0, _info.width, _info.height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 3. render lights on top of scene
		// --------------------------------
		_shaderLightBox->use();
		_shaderLightBox->setMat4("projection", projection);
		_shaderLightBox->setMat4("view", view);
		for (unsigned int i = 0; i < _lightPositions.size(); i++)
		{
			model = glm::mat4();
			model = glm::translate(model, _lightPositions[i]);
			model = glm::scale(model, glm::vec3(0.125f));
			_shaderLightBox->setMat4("model", model);
			_shaderLightBox->setVec3("lightColor", _lightColors[i]);
			renderCube();
		}
	}

	void renderCube()
	{
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

	void shutdown()
	{
		Application::shutdown();
	}
private:
	Shader* _shaderGemotryPass;
	Shader* _shaderLightingPass;
	Shader* _shaderLightBox;

	Model* nanosuit;

	unsigned int gPosition, gNormal, gAlbedoSpec;

	unsigned int _gBuffer;
	std::vector<glm::vec3> _lightPositions;
	std::vector<glm::vec3> _lightColors;
	std::vector<glm::vec3> _objectPositions;

	unsigned int _cubeVAO;
	unsigned int _cubeVBO;

	unsigned int _quadVAO;
	unsigned int _quadVBO;
};

DEBUG_MAIN(DeferredRender01);