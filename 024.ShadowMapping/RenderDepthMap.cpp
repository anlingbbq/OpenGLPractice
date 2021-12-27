#define STB_IMAGE_IMPLEMENTATION
#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <Minor/Shape/ColorCube.h>

#define DEBUG_MODE false

using namespace minor;
class RenderDepthMap : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "RenderDepthMap";
		_info.width = 1280;
		_info.height = 720;
		_camera = ObserveCamera::create(_info.width, _info.height);
	}
	
	void startup()
	{
		Application::startup();

#if DEBUG_MODE == true
		_debugColorShader = new Shader("normal.vs", "constant.fs");
		_debugLightCube = ColorCube::create(glm::vec3(1, 1, 1));
		_debugLightCube->setPosition(_lightPos);
		_debugLightCube->setScale(0.2f);
		_debugTargetCube = ColorCube::create(glm::vec3(1, 0, 0));
		_debugTargetCube->setScale(0.2f);
#else
		_simpleDepthShader = new Shader("shadowMappingDepth.vs", "shadowMappingDepth.fs");
		_debugDepthQuadShader = new Shader("debugQuad.vs", "debugQuad.fs");
#endif
		// plane VAO
		glGenVertexArrays(1, &_planeVAO);
		glGenBuffers(1, &_planeVBO);
		glBindVertexArray(_planeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, _planeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_planeVertices), _planeVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindVertexArray(0);

		// cube VAO
		glGenVertexArrays(1, &_cubeVAO);
		glGenBuffers(1, &_cubeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, _cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_cubeVertices), _cubeVertices, GL_STATIC_DRAW);
		glBindVertexArray(_cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// plane texture
		_planeTex = TextureLoader::Inst()->LoadTexture2D("wood.png");

#if DEBUG_MODE == false
		// configure depth map FBO
		glGenFramebuffers(1, &_depthMapFBO);
		// create depth texture
		glGenTextures(1, &_depthMap);
		glBindTexture(GL_TEXTURE_2D, _depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _shadowMapWidth, _shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// attach depth texture as FBO's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// shader configuration
		_debugDepthQuadShader->use();
		_debugDepthQuadShader->setInt("depthMap", 0);

		// quad VAO
		glGenVertexArrays(1, &_quadVAO);
		glGenBuffers(1, &_quadVBO);
		glBindVertexArray(_quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_quadVertices), &_quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
#endif
	}
	
	void render()
	{
		Application::render();

#if DEBUG_MODE == true
		glViewport(0, 0, _info.width, _info.height);
		_debugLightCube->drawShape();
		_debugTargetCube->drawShape();
		_debugColorShader->use();
		_debugColorShader->setMat4("view", this->getCameraView());
		_debugColorShader->setMat4("projection", this->getCameraProjection());
		renderScene(_debugColorShader);
#else
		// 1.render depth of scene to texture
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float nearPlane = 1.0f, farPlane = 7.5f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
		lightView = glm::lookAt(_lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightSpaceMatrix = lightProjection * lightView;
		_simpleDepthShader->use();
		_simpleDepthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, _shadowMapWidth, _shadowMapHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _planeTex);
		renderScene(_simpleDepthShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// reset viewport
		glViewport(0, 0, _info.width, _info.height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render depth map to quad for visual debugging
		_debugDepthQuadShader->use();
		_debugDepthQuadShader->setFloat("nearPlane", nearPlane);
		_debugDepthQuadShader->setFloat("farPlane", farPlane);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _depthMap);
		glBindVertexArray(_quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
#endif
	}

	void renderScene(Shader* shader)
	{
		// floor
		glm::mat4 model;
		shader->setMat4("model", model);
		shader->setVec3("objectColor", glm::vec3(0.3, 0.3, 0.3));
		glBindVertexArray(_planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// cubes
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));
		shader->setMat4("model", model);
		shader->setVec3("objectColor", glm::vec3(0, 0.3, 0.3));
		glBindVertexArray(_cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(0.5f));
		shader->setMat4("model", model);
		glBindVertexArray(_cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
		model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(0.25));
		shader->setMat4("model", model);
		glBindVertexArray(_cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

	void shutdown()
	{
		Application::shutdown();
	}
private:
	float _planeVertices[48] =
	{
		 // positions           // normals         // texcoords
		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
		 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 10.0f
	};
	unsigned int _planeVAO;
	unsigned int _planeVBO;
	unsigned int _planeTex;

	float _cubeVertices[288] =
	{
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
	unsigned int _cubeVAO;
	unsigned int _cubeVBO;

	float _quadVertices[20] =
	{
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	unsigned int _quadVAO;
	unsigned int _quadVBO;

	unsigned int _shadowMapHeight = 1024;
	unsigned int _shadowMapWidth = 1024;
	glm::vec3 _lightPos = glm::vec3(-2.0f, 4.0f, -1.0f);

	unsigned int _depthMapFBO;
	unsigned int _depthMap;

	Shader* _debugColorShader;
	ColorCube* _debugLightCube;
	ColorCube* _debugTargetCube;
	
	Shader* _simpleDepthShader;
	Shader* _debugDepthQuadShader;
};

//DEBUG_MAIN(RenderDepthMap);