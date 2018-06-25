#ifndef __TextureCube_H__
#define __TextureCube_H__

#include "ShapeBase.h"
#include <Minor/TextureLoader.h>

namespace minor
{
	static float texture_cube_vertices[] = {
		 // Back face
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		 // Front face
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		 // Left face
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		 // Right face
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
		 // Bottom face
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		 // Top face
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
	};

	static char* texture_cube_vs = {
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

	static char* texture_cube_fs = {
		"#version 430 core								\n"
		"uniform sampler2D ourTexture;					\n"
		"out vec4 fragColor;							\n"
		"in vec2 TexCoord;								\n"
		"void main()									\n"
		"{												\n"
		"	fragColor = texture(ourTexture, TexCoord);	\n"
		"}												\n"
	};

	class TextureCube : public ShapeBase
	{
	public:
		static TextureCube* create(const std::string texPath)
		{
			TextureCube* cube = new TextureCube(texture_cube_vertices, sizeof(texture_cube_vertices));
			cube->init();
			cube->bindTexture(texPath);
			return cube;
		}

		static TextureCube* create()
		{
			TextureCube* cube = new TextureCube(texture_cube_vertices, sizeof(texture_cube_vertices));
			cube->init();
			return cube;
		}

		TextureCube(float* vertices, int verSize)
		{
			_vertices = vertices;
			_verSize = verSize;
		}

		virtual void init()
		{
			ShapeBase::init();
			_trans = glm::mat4(1.0f);
			_changeTrans = true;
		}

		virtual void interpret()
		{
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
		}

		void bindTexture(const std::string texPath)
		{
			_textureId = TextureLoader::Inst()->LoadTexture2D(texPath.c_str());

			_shader = new Shader(texture_cube_vs, texture_cube_fs, DATA);
			_shader->use();
			_shader->setInt("ourTexture", 0);
			glActiveTexture(GL_TEXTURE0);
		}

		virtual void drawShape()
		{
			glBindVertexArray(_vao);

			_shader->use();
			changeView();
			changeModel();

			glBindTexture(GL_TEXTURE_2D, _textureId);

			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}

		virtual void clear()
		{
			glDeleteVertexArrays(1, &_vao);
			glDeleteBuffers(1, &_vbo);

			delete _shader;
			delete this;
		}

	protected:
		unsigned int _textureId;
	};
};

#endif // !__TextureCube_H__

