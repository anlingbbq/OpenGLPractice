#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include "Rectangle.h"
#include <Minor/TextureLoader.h>

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

		wall_id = TextureLoader::Inst()->LoadTexture2D("wall.jpg");
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		awesome_id = TextureLoader::Inst()->LoadTexture2D("awesomeface.png");
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
		glBindTexture(GL_TEXTURE_2D, wall_id);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, awesome_id);
		
		_shader->use();

		_rect->bindVertexArray();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void shutdown()
	{
		_rect->clear();
		delete _shader;
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