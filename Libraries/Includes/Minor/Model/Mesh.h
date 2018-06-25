#ifndef	__Mesh_H__
#define __Mesh_H__

#include <vector>
#include <stddef.h>
#include <Minor/Shader.h>
#include <Minor/Base/MComponent.h>
//#include <Minor/TextureManager.h>

using namespace std;

namespace minor
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
		float random;
		//glm::vec3 tangent;
		//glm::vec3 bitangent;
	};

	struct Texture
	{
		unsigned int id;
		string type;
		string path;
	};

	const string texture_diffuse = "texture_diffuse";
	const string texture_specular = "texture_specular";
	const string texture_normal = "texture_normal";
	const string texture_height = "texture_height";

	class Mesh : public MComponent
	{
	public:
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;
		unsigned int vao;

		Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
		{
			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;

			this->init();
		}

		void Draw(Shader* shader)
		{
			unsigned int diffuseNum = 1;
			unsigned int specluarNum = 1;
			//unsigned int normalNum = 1;
			//unsigned int heightNum = 1;
			for (unsigned int i = 0; i < textures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				string number;
				string name = textures[i].type;
				if (name == texture_diffuse)
					number = std::to_string(diffuseNum++);
				else if (name == texture_specular)
					number = std::to_string(specluarNum++);
				//else if (name == texture_normal)
					//number = std::to_string(normalNum++);
				//else if (name == texture_height)
					//number = std::to_string(heightNum++);

				shader->setInt(("material." + name + number).c_str(), i);
				glBindTexture(GL_TEXTURE_2D, textures[i].id);
				//TextureManager::Inst()->BindTexture(textures[i].id);
			}

			glActiveTexture(GL_TEXTURE0);

			// draw mesh
			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

	private:
		unsigned int _vbo, _ebo;

		void init()
		{
			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &_vbo);
			glGenBuffers(1, &_ebo);

			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, _vbo);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

			// vertex positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			// vertex normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
			// vertex texture coords
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
			// extra paramter random
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, random));
			// vertex tangent
			//glEnableVertexAttribArray(3);
			//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
			// vertex bitangent
			//glEnableVertexAttribArray(4);
			//glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

			glBindVertexArray(0);
		}
	};
}

#endif // __Mesh_H__
