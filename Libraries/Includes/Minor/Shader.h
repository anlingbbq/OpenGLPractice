#ifndef __SHADER_H__
#define __SHADER_H__

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>

namespace minor
{
	enum DataType {
		FILE,
		DATA
	};

	class Shader
	{
	public:
		unsigned int id;

		Shader(std::string vertexData, std::string fragmentData, DataType type = FILE)
		{
			if (type == FILE)
			{
				// load shader data
				std::string vertexCode;
				std::string fragmentCode;

				std::ifstream vShaderFile;
				std::ifstream fShaderFile;

				vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
				fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

				try
				{
					_vsFileName = _directory + vertexData;
					_fsFileName = _directory + fragmentData;
					vShaderFile.open(_vsFileName);
					fShaderFile.open(_fsFileName);

					std::stringstream vShaderStream, fShaderStream;

					vShaderStream << vShaderFile.rdbuf();
					fShaderStream << fShaderFile.rdbuf();

					vShaderFile.close();
					fShaderFile.close();

					vertexCode = vShaderStream.str();
					fragmentCode = fShaderStream.str();
				}
				catch (std::ifstream::failure e)
				{
					printf("shader error: file not successfully read\n%s\n%s\n", _vsFileName.c_str(), _fsFileName.c_str());
				}

				vertexData = vertexCode.c_str();
				fragmentData = fragmentCode.c_str();

				create(vertexData.c_str(), fragmentData.c_str());
			}
			else if (type == DATA)
			{
				create(vertexData.c_str(), fragmentData.c_str());
			}
		}
		Shader(std::string vertexData, std::string fragmentData, std::string geometryData, DataType type = FILE)
		{
			new (this)Shader(vertexData, fragmentData, type);
			LoadGeometryShader(geometryData, type);
		}
		
		void create(const GLchar* vertexData, const GLchar* fragmentData)
		{
			unsigned int vertex, fragment;

			//printf("vertex shader:\n%s\n", vertexData);
			vertex = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex, 1, &vertexData, NULL);
			glCompileShader(vertex);
			checkCompileErrors(vertex, "vertex");

			//printf("fragment shader:\n%s\n", fragmentData);
			fragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment, 1, &fragmentData, NULL);
			glCompileShader(fragment);
			checkCompileErrors(fragment, "fragment");

			id = glCreateProgram();
			glAttachShader(id, vertex);
			glAttachShader(id, fragment);
			glLinkProgram(id);
			checkCompileErrors(id, "program");

			glDeleteShader(vertex);
			glDeleteShader(fragment);
		}
		void use()
		{
			glUseProgram(id);
		}

		void setBool(const std::string &name, bool value) const
		{
			glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
		}
		void setInt(const std::string &name, int value) const
		{
			glUniform1i(glGetUniformLocation(id, name.c_str()), value);
		}
		void setFloat(const std::string &name, float value) const
		{
			glUniform1f(glGetUniformLocation(id, name.c_str()), value);
		}
		void setVec2(const std::string &name, glm::vec2 value) const
		{
			glUniform2f(glGetUniformLocation(id, name.c_str()), value.x, value.y);
		}
		void setVec3(const std::string &name, glm::vec3 value) const
		{
			glUniform3f(glGetUniformLocation(id, name.c_str()), value.r, value.g, value.b);
		}
		void setVec4(const std::string &name, glm::vec4 value) const
		{
			glUniform4f(glGetUniformLocation(id, name.c_str()), value.r, value.g, value.b, value.a);
		}
		void setMat3(const std::string &name, glm::mat3 value) const
		{
			glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
		}
		void setMat4(const std::string &name, glm::mat4 value) const
		{
			glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
		}

		void LoadGeometryShader(std::string geometryData, DataType type = FILE)
		{
			if (type == FILE)
			{
				// load shader data
				std::string geometryCode;
				std::ifstream gShaderFile;
				gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

				try
				{
					_gsFileName = _directory + geometryData;
					gShaderFile.open(_gsFileName);
					std::stringstream gShaderStream;
					gShaderStream << gShaderFile.rdbuf();

					gShaderFile.close();
					geometryCode = gShaderStream.str();
				}
				catch (std::ifstream::failure e)
				{
					printf("shader error: file not successfully read");
				}
				geometryData = geometryCode.c_str();
				CreateGeometryShader(geometryData.c_str());
			}
			else
			{
				CreateGeometryShader(geometryData.c_str());
			}
		}
		void CreateGeometryShader(const char* geometryData)
		{
			//printf("geometry shader:\n%s\n", geometryData);

			unsigned int geometry;
			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &geometryData, NULL);
			glCompileShader(geometry);
			glAttachShader(id, geometry);
			glLinkProgram(id);
			checkCompileErrors(geometry, "geometry");

			glDeleteShader(geometry);
		}

	private:
		std::string _vsFileName;
		std::string _fsFileName;
		std::string _gsFileName;
		std::string _directory = "../Resources/Shader/";
		void checkCompileErrors(unsigned int shader, std::string type)
		{
			int success;
			char infoLog[1024];
			if (type != "program")
			{
				glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					std::string fileName;
					fileName = type == "vertex" ? _vsFileName : type == "fragment" ? _fsFileName : _gsFileName;
					glGetShaderInfoLog(shader, 1024, NULL, infoLog);
					printf("%s shader compilation error: \npath:%s\n%s\n", type.c_str(), fileName.c_str(), infoLog);
				}
			}
			else
			{
				glGetProgramiv(shader, GL_LINK_STATUS, &success);
				if (!success)
				{
					glGetProgramInfoLog(shader, 1024, NULL, infoLog);
					printf("%s shader linking error: \n%s", type.c_str(), infoLog);
				}
			}
		}
	};
};

#endif // !__SHADER_H__