#ifndef __TexturePlane_H__
#define __TexturePlane_H__

#include "ShapeBase.h"
#include "../TextureLoader.h"

namespace minor
{
	static float texture_plane_vertices[] = {
		 // positions            // normals         // texcoords
		 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
		-10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

		 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
		 10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
	};

	static char* texture_plane_vs = {
		"#version 430 core												\n"
		"layout(location = 0) in vec3 pos;								\n"
		"layout(location = 2) in vec2 texCoord;							\n"
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

	static char* texture_plane_fs = {
		"#version 430 core								\n"
		"uniform sampler2D ourTexture;					\n"
		"out vec4 fragColor;							\n"
		"in vec2 TexCoord;								\n"
		"void main()									\n"
		"{												\n"
		"	fragColor = texture(ourTexture, TexCoord);	\n"
		"}												\n"
	};

	class TexturePlane : public ShapeBase
	{
	public:
		static TexturePlane* create(const std::string texPath)
		{
			TexturePlane* plane = new TexturePlane(texture_plane_vertices, sizeof(texture_plane_vertices));
			plane->init();
			plane->bindTexture(texture_plane_vs, texture_plane_fs, texPath, DATA);
			return plane;
		}

		static TexturePlane* create(char* fs, const std::string texPath)
		{
			TexturePlane* plane = new TexturePlane(texture_plane_vertices, sizeof(texture_plane_vertices));
			plane->init();
			plane->bindTexture("common.vs", fs, texPath, FILE);
			return plane;
		}

		TexturePlane(float* vertices, int verSize)
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
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(2);
		}

		void bindTexture(char* vs, char* fs, const std::string texPath, DataType type)
		{
			textureId = TextureLoader::Inst()->LoadTexture2D(texPath.c_str());

			_shader = new Shader(vs, fs, type);
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

			glBindTexture(GL_TEXTURE_2D, textureId);

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

		unsigned int textureId;
	};
};

#endif // !__TexturePlane_H__

