#ifndef __ObserveCamera_H__
#define __ObserveCamera_H__

#include <Minor/CameraBase.h>

namespace minor
{
	class ObserveCamera : public CameraBase
	{
	public:
		static ObserveCamera* create(int width, int height)
		{
			ObserveCamera* camera = new ObserveCamera();
			camera->_yaw = -90.0f;
			camera->_pitch = 0.0f;
			camera->_width = width;
			camera->_height = height;
			camera->_znear = 0.1f;
			camera->_zfar = 100.0f;

			return camera;
		}

		virtual void update(float deltaTime)
		{
			GLfloat speed = _moveSpeed * deltaTime;

			if (_keys[GLFW_KEY_W] || _keys[GLFW_KEY_UP])
			{
				_pos += speed * _front;
				_changeTrans = true;
			}
			if (_keys[GLFW_KEY_S] || _keys[GLFW_KEY_DOWN])
			{
				_pos -= speed * _front;
				_changeTrans = true;
			}

			if (_keys[GLFW_KEY_A] || _keys[GLFW_KEY_LEFT])
			{
				_pos -= glm::normalize(glm::cross(_front, _up)) * speed;
				_changeTrans = true;
			}
			if (_keys[GLFW_KEY_D] || _keys[GLFW_KEY_RIGHT])
			{
				_pos += glm::normalize(glm::cross(_front, _up)) * speed;
				_changeTrans = true;
			}

			if (_keys[GLFW_KEY_LEFT_SHIFT])
				_moveSpeed = 0.1f * _moveSpeedOrg;
			else
				_moveSpeed = _moveSpeedOrg;
		}

		virtual void onKey(int key, int action)
		{
			if (key >= 0 && key < 512)
			{
				if (action == GLFW_PRESS)
					_keys[key] = true;
				else if (action == GLFW_RELEASE)
					_keys[key] = false;
			}
		}

		virtual void onMouseButton(int button, int action)
		{
			if (button >= 0 && button < 512)
			{
				if (action == GLFW_PRESS)
					_keys[button] = true;
				else if (action == GLFW_RELEASE)
					_keys[button] = false;
			}
		}

		virtual void onMouseMove(int posx, int posy)
		{
			if (_keys[GLFW_MOUSE_BUTTON_3])
			{
				float yoffset = posy - _ylast;
				float xoffset = posx - _xlast;

				_xlast = (float)posx;
				_ylast = (float)posy;

				_pos += yoffset * _mouseMoveSpeed * _moveSpeed * _up;
				_pos -= glm::normalize(glm::cross(_front, _up)) * xoffset * _mouseMoveSpeed * _moveSpeed;

				_changeTrans = true;
			}
			else if (!_keys[GLFW_MOUSE_BUTTON_2])
			{
				_xlast = (float)posx;
				_ylast = (float)posy;
				return;
			}

			if (_firstMouse)
			{
				_xlast = (float)posx;
				_ylast = (float)posy;
				_firstMouse = false;
			}

			float xoffset = posx - _xlast;
			float yoffset = _ylast - posy;
			_xlast = (float)posx;
			_ylast = (float)posy;

			_yaw += xoffset * _rotateSpeed;
			_pitch += yoffset * _rotateSpeed;

			if (_pitch > 89.0f)
				_pitch = 89.0f;
			if (_pitch < -89.0f)
				_pitch = -89.0f;

			glm::vec3 front;
			front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
			front.y = sin(glm::radians(_pitch));
			front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
			_front = glm::normalize(front);

			_changeTrans = true;
		}

		virtual void onMouseWheel(int xoffset, int yoffset)
		{
			if (_keys[GLFW_KEY_LEFT_CONTROL])
			{
				if (_fov >= 1.0f && _fov <= 45.0f)
					_fov -= yoffset * 2;

				if (_fov <= 1.0f)
					_fov = 1.0f;
				if (_fov >= 45.0f)
					_fov = 45.0f;
			}
			else 
			{
				_pos += (float)yoffset * _front * _moveSpeed;
			}

			_changeTrans = true;
		}
	private:
		bool _keys[512];

		float _xlast;
		float _ylast;
		float _yaw;
		float _pitch;

		bool _firstMouse = true;

		float _mouseMoveSpeed = 0.01f;
	};
};

#endif // !__ObserveCamera_H__
