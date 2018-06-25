#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include "Rectangle.h"
#include <Minor/TextureManager.h>

using namespace minor;
class MixTexture : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "MixTexture";
	}
	
	void startup()
	{
		_shader = new Shader("mixTexture.vs", "mixTexture.fs");
		_rect = Rectangle::create(_vertices, _indices, sizeof(_vertices), sizeof(_indices));
		_rect->allot();
		// interpret
		glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		_rect->unbind();

		TextureManager::Inst()->LoadTexture("../Resources/wall.jpg", wall_id, GL_BGR, GL_RGB);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		TextureManager::Inst()->LoadTexture("../Resources/awesomeface.png", awesome_id, GL_BGRA, GL_RGB);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		_shader->use();
		_shader->setInt("texture1", 0);
		_shader->setInt("texture2", 1);
	}
	
	void render()
	{
		glActiveTexture(GL_TEXTURE0);
		TextureManager::Inst()->BindTexture(wall_id);
		glActiveTexture(GL_TEXTURE1);
		TextureManager::Inst()->BindTexture(awesome_id);
		
		_shader->use();

		_rect->bindVertexArray();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void shutdown()
	{
		_rect->clear();
		delete _shader;
		TextureManager::Inst()->UnloadAllTextures();
	}
private:
	minor::Rectangle* _rect;
	Shader* _shader;

	int wall_id = 1001;
	int awesome_id = 1002;

	float _vertices[20] = {
		 // position		// texture coords
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	unsigned int _indices[6] = {
		0, 1, 3,
		1, 2, 3
	};
};

//DEBUG_MAIN(MixTexture);