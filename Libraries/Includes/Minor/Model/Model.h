#ifndef	__Model_H__
#define __Model_H__

#include <vector>
#include <Minor/Model/Mesh.h>
#include <Minor/Shader.h>
#include <Minor/TextureLoader.h>
//#include <Minor/TextureManager.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

namespace minor
{
	class Model : public MComponent
	{
	public:
		vector<Texture> texture_loaded;
		vector<Mesh> meshes;
		string directory;

		Model(string path, bool flipUv = true)
		{
			path = _directory + path;
			init(path, flipUv);
		}

		void Draw(Shader* shader)
		{
			for (unsigned int i = 0; i < meshes.size(); i++)
			{
				meshes[i].Draw(shader);
			}
		}
	private:
		string _directory = "../Resources/Model/";
		void init(string path, bool flipUv)
		{
			Assimp::Importer importer;
			const aiScene* scene;
			if (flipUv)
				scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
			else
				scene = importer.ReadFile(path, aiProcess_Triangulate);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				cout << "assimp error:" << importer.GetErrorString() << endl;
				return;
			}
			directory = path.substr(0, path.find_last_of('/')) + "/";
			processNode(scene->mRootNode, scene);
		}

		void processNode(aiNode *node, const aiScene* scene)
		{
			// process all the node's meshes
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				meshes.push_back(processMesh(mesh, scene));
			}

			// then do the same for each of its children
			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				processNode(node->mChildren[i], scene);
			}
		}

		Mesh processMesh(aiMesh *mesh, const aiScene* scene)
		{
			vector<Vertex> vertices;
			vector<unsigned int> indices;
			vector<Texture> textures;

			// process vertex position, normals and texCoord
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				Vertex vertex;
				glm::vec3 vector;
				// position
				vector.x = mesh->mVertices[i].x;
				vector.y = mesh->mVertices[i].y;
				vector.z = mesh->mVertices[i].z;
				vertex.position = vector;
				// normals
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.normal = vector;
				// texCoord
				if (mesh->mTextureCoords[0])
				{
					glm::vec2 vec;
					vec.x = mesh->mTextureCoords[0][i].x;
					vec.y = mesh->mTextureCoords[0][i].y;
					vertex.texCoords = vec;
				}
				else
				{
					vertex.texCoords = glm::vec2(0.0f, 0.0f);
				}
				// tangent
				/*vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.tangent = vector;*/
				// bitangent
				/*vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				vertex.bitangent = vector;*/
				// extra paramter 
				vertex.random = Randomf() * 40.0f;

				vertices.push_back(vertex);
			}

			// process indices
			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; j++)
				{
					indices.push_back(face.mIndices[j]);
				}
			}

			// process material
			if (mesh->mMaterialIndex >= 0)
			{
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
				// 1.diffuse maps
				vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, texture_diffuse);
				textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
				// 2.specular maps
				vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, texture_specular);
				textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
				// 3.normal maps
				//std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, texture_normal);
				//textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
				// 4. height maps
				//std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, texture_height);
				//textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
			}

			return Mesh(vertices, indices, textures);
		}

		vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
		{
			vector<Texture> textures;
			for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
			{
				aiString str;
				mat->GetTexture(type, i, &str);
				bool skip = false;
				for (unsigned int j = 0; j < texture_loaded.size(); j++)
				{
					if (strcmp(texture_loaded[j].path.data(), str.C_Str()) == 0)
					{
						textures.push_back(texture_loaded[j]);
						skip = true;
						break;
					}
				}
				if (!skip)
				{
					Texture texture;
					texture.id = TextureLoader::Inst()->LoadTexture2D(str.C_Str(), directory);
					//string filename = string(str.C_Str());
					//filename = directory + "/" + filename;
					//texture.id = TextureManager::Inst()->LoadTexture2D(filename);
					texture.type = typeName;
					texture.path = str.C_Str();
					textures.push_back(texture);
					texture_loaded.push_back(texture);
				}
			}
			return textures;
		}
	};
}

#endif // __Model_H__
