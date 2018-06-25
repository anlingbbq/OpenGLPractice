#ifndef __TextureQuad_H__
#define __TextureQuad_H__

#include "MRectangle.h"
#include <Minor/TextureLoader.h>

namespace minor
{
	static float texture_quad_vertices[] = {
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	static unsigned int texture_quad_indices[6] = {
		0, 3, 1,
		3, 2, 1
	};

	static char* texture_quad_vs = {
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

	static char* texture_quad_fs = {
		"#version 430 core									\n"
		"uniform sampler2D ourTexture;						\n"
		"out vec4 fragColor;								\n"
		"in vec2 TexCoord;									\n"
		"void main()										\n"
		"{													\n"
		"	fragColor = texture(ourTexture, TexCoord);		\n"
		"}													\n"
	};

	class TextureQuad : public MRectangle
	{
	public:
		static TextureQuad* create(const std::string texPath, const char* fsCode = "")
		{
			TextureQuad* cube = new TextureQuad(texture_quad_vertices, texture_quad_indices, sizeof(texture_quad_vertices), sizeof(texture_quad_indices));
			cube->init();
			cube->bindTexture(texPath, fsCode);
			return cube;
		}

		TextureQuad(float* vertices, unsigned int* indices, int verSize, int indSize) : MRectangle(vertices, indices, verSize, indSize) {}

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

		void bindTexture(const std::string texPath, const char* fsCode)
		{
			_textureId = TextureLoader::Inst()->LoadTexture2D(texPath.c_str());

			if (fsCode == "") fsCode = texture_quad_fs;
			_shader = new Shader(texture_quad_vs, fsCode, DATA);
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

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

#endif // !__TextureQuad_H__

