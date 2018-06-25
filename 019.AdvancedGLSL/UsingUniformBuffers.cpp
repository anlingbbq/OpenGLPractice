#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include <Minor/ObserveCamera.h>
#include <Minor/Shape/ColorCube.h>

using namespace minor;
class UsingUniformBuffers : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "UsingUniformBuffers";
		_camera = ObserveCamera::create(_info.width, _info.height);
	}
	
	void startup()
	{
		Application::startup();

		// configure a uniform buffer object
#pragma region old approach to bind point in 3.3 and backwards
		// first. We get the relevant block indices
		/*unsigned int uniformBlockIndexRed = glGetUniformBlockIndex(shaderRed.ID, "Matrices");
		unsigned int uniformBlockIndexGreen = glGetUniformBlockIndex(shaderGreen.ID, "Matrices");
		unsigned int uniformBlockIndexBlue = glGetUniformBlockIndex(shaderBlue.ID, "Matrices");
		unsigned int uniformBlockIndexYellow = glGetUniformBlockIndex(shaderYellow.ID, "Matrices");*/
		// then we link each shader's uniform block to this uniform binding point
		/*glUniformBlockBinding(shaderRed.ID, uniformBlockIndexRed, 0);
		glUniformBlockBinding(shaderGreen.ID, uniformBlockIndexGreen, 0);
		glUniformBlockBinding(shaderBlue.ID, uniformBlockIndexBlue, 0);
		glUniformBlockBinding(shaderYellow.ID, uniformBlockIndexYellow, 0);*/
#pragma endregion

		glGenBuffers(1, &_ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, _ubo, 0, 2 * sizeof(glm::mat4));

		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(_camera->getProjection()));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		_grayCube = ColorCube::create(glm::vec3(0.2f, 0.3f, 0.3f), "normal-std140.vs", "constant.fs");
		_grayCube->setPosition(glm::vec3(-0.75f, 0.75f, 0));
		_orangeCube = ColorCube::create(glm::vec3(1.0f, 0.5f, 0.2f), "normal-std140.vs", "constant.fs");
		_orangeCube->setPosition(glm::vec3(0.75f, 0.75f, 0));
		_pinkCube = ColorCube::create(glm::vec3(1.0f, 0, 1.0f), "normal-std140.vs", "constant.fs");
		_pinkCube->setPosition(glm::vec3(-0.75f, -0.75f, 0));
		_blueCube = ColorCube::create(glm::vec3(0.2f, 0.5f, 1.0f), "normal-std140.vs", "constant.fs");
		_blueCube->setPosition(glm::vec3(0.75f, -0.75f, 0));
	}
	
	void render()
	{
		Application::render();

		// set the view matrix in uniform block - we only have to do this once per loop iteration.
		glm::mat4 view = _camera->getView();
		glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(_camera->getView()));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		_grayCube->drawShapeOptimize();
		_orangeCube->drawShapeOptimize();
		_pinkCube->drawShapeOptimize();
		_blueCube->drawShapeOptimize();
	}

	void shutdown()
	{
		Application::shutdown();
	}
private:
	unsigned int _ubo;

	ColorCube* _grayCube;
	ColorCube* _orangeCube;
	ColorCube* _pinkCube;
	ColorCube* _blueCube;
};

DEBUG_MAIN(UsingUniformBuffers);