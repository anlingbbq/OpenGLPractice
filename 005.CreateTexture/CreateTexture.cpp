#define STB_IMAGE_IMPLEMENTATION
#include <Minor/Minor.h>
#include <Minor/Shader.h>
#include "Rectangle.h"
#include <Minor/TextureLoader.h>

using namespace minor;
class CreateTexture : public Application
{
public:
	void init()
	{
		Application::init();
		_info.title = "CreateTexture";
	}
	
	void startup()
	{
		_shader = new Shader("textureWithoutNormal.vs", "texture.fs");

		_rect = Rectangle::create(_vertices, _indices, sizeof(_vertices), sizeof(_indices));
		_rect->allot();
		// interpret
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		_rect->unbind();

		// load image, create texture and generater mipmaps
		glGenTextures(1, &_texture);
		glBindTexture(GL_TEXTURE_2D, _texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		_texture = TextureLoader::Inst()->LoadTexture2D("wall.jpg");
	}
	
	void render()
	{
		glBindTexture(GL_TEXTURE_2D, _texture);
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
	float _vertices[20] = {
		// position		   // texture coords
		0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
	   -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	unsigned int _indices[6] = {
		0, 1, 3,
		1, 2, 3
	};

	GLuint _texture;

	Shader* _shader;
	minor::Rectangle* _rect;
};

DEBUG_MAIN(CreateTexture);