#ifndef _TextureLoader_H__
#define _TextureLoader_H__

#include "stb_image.h"

#include <string>
#include <vector>
#include <iostream>

namespace minor
{
	class TextureLoader
	{
	public:
		static TextureLoader* inst;
		static TextureLoader* Inst()
		{
			if (inst == nullptr)
				inst = new TextureLoader();

			return inst;
		}

		int LoadTexture2D(std::string filename, std::string directory = "")
		{
			if (directory == "") filename = _directory + filename;
			else filename = directory + filename;

			unsigned int textureID;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);

			int width, height, channels;
			unsigned char *data = stbi_load(filename.c_str(), &width, &height, &channels, 0);

			if (data)
			{
				GLenum format;
				if (channels == 1)
					format = GL_RED;
				else if (channels == 3)
					format = GL_RGB;
				else if (channels == 4)
					format = GL_RGBA;

				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else
			{
				std::cout << "TextureLoader error:Failed to load texture\n" << std::endl;
			}
			stbi_image_free(data);

			return textureID;
		}

		int LoadCubemap(std::vector<std::string> faces)
		{
			GLuint textureID;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

			int width, height, channels;
			std::string filename;
			for (unsigned int i = 0; i < faces.size(); i++)
			{
				filename = _directory + '/' + faces[i];
				unsigned char *data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
				if (data != nullptr)
				{
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
					stbi_image_free(data);
				}
				else
				{
					printf("load texture error: failed to load at path : %s\n", filename.c_str());
					stbi_image_free(data);
				}
			
			}

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			return textureID;
		}
	
		std::string getDir()
		{
			return _directory;
		}

		void setDir(std::string dirctory)
		{
			_directory = dirctory;
		}
	private:
		TextureLoader() {};
		std::string _directory = "../Resources/Texture/";
	};
}

#endif // !_TextureLoader_H__
