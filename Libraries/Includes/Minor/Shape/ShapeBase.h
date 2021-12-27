#ifndef __ShapeBase_H__
#define __ShapeBase_H__

#include <Minor/Shader.h>
#include <Minor/Base/GameObject.h>

namespace minor
{
	class ShapeBase : public GameObject
	{
	public:
		static ShapeBase* create()
		{
			// do something for example
			// ShapeBase* base = new ShapeBase();
			// init();
			// return base;

			return nullptr;
		}

		virtual void init()
		{
			allot();
			interpret();
			unbind();

			_trans = glm::mat4(1.0f);
			_changeTrans = true;
		}

		virtual void allot() 
		{
			glGenVertexArrays(1, &_vao);
			glGenBuffers(1, &_vbo);

			glBindBuffer(GL_ARRAY_BUFFER, _vbo);
			glBufferData(GL_ARRAY_BUFFER, _verSize, _vertices, GL_STATIC_DRAW);

			glBindVertexArray(_vao);
		};

		virtual void interpret() {};

		virtual void unbind() 
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		};

		void bindVertexArray()
		{
			glBindVertexArray(_vao);
		}

		virtual void drawShape() {};

		Shader* getShader()
		{
			return _shader;
		}

		void setShader(Shader* shader)
		{
			_shader = shader;
		}

#pragma region transform
		void setScale(glm::vec3 scale)
		{
			_scale = scale;
			_changeTrans = true;
		}

		void setScale(float scale)
		{
			_scale = glm::vec3(scale, scale, scale);
			_changeTrans = true;
		}

		glm::vec3 getScale()
		{
			return _scale;
		}

		glm::quat setRotate(float angle, glm::vec3 axis)
		{
			_rotate = _orgRotate = glm::angleAxis(glm::radians(angle), axis);
			_changeTrans = true;
			return _rotate;
		}

		glm::quat setRotate(glm::quat r1, glm::quat r2)
		{
			_rotate = r1 * r2;
			_changeTrans = true;
			return _rotate;
		}

		glm::quat getRotate()
		{
			return _rotate;
		}

		void setPosition(glm::vec3 pos)
		{
			_position = pos;
			_changeTrans = true;
		}

		glm::vec3 getPosition()
		{
			return _position;
		}

		void setView(glm::vec3 pos)
		{
			glm::mat4 view;
			view = glm::translate(view, pos);
			_shader->setMat4("view", view);
		}

		void setProjection(float fov, float aspect, float znear, float zfar)
		{
			glm::mat4 projection;
			projection = glm::perspective(glm::radians(fov), aspect, znear, zfar);
			_shader->setMat4("projection", projection);
		}

		void changeModel()
		{
			if (_changeTrans)
			{
				glm::mat4 trans;
				_trans = glm::translate(trans, _position);
				_trans = glm::rotate(_trans, glm::angle(_rotate), glm::axis(_rotate));
				_trans = glm::scale(_trans, _scale);

				_shader->setMat4("model", _trans);
				
				_changeTrans = false;
			}
		}

		void changeView()
		{
			if (Application::app->getCameraChangeTrans())
			{
				_shader->setMat4("view", Application::app->getCameraView());
				_shader->setMat4("projection", Application::app->getCameraProjection());
			}
		}

#pragma endregion
	protected:
		Shader* _shader;

		unsigned int _vao;
		unsigned int _vbo;

		int _verSize;
		float* _vertices;

		glm::vec3 _position = glm::vec3(0);
		glm::vec3 _scale = glm::vec3(1);
		glm::quat _rotate = glm::angleAxis(0.0f, glm::vec3(0, 0, 1));

		glm::quat _orgRotate = glm::angleAxis(0.0f, glm::vec3(0, 0, 1));

		glm::mat4 _trans;

		bool _changeTrans;
	};
};

#endif // !__ShapeBase_H__
