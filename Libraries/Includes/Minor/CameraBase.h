#ifndef __CameraBase_H__
#define __CameraBase_H__

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

namespace minor
{
	class CameraBase
	{
	public:
		virtual void update(float deltaTime)
		{

		}

		virtual void onKey(int key, int action)
		{

		}

		virtual void onMouseButton(int button, int action)
		{

		}

		virtual void onMouseMove(int posX, int posY)
		{

		}

		virtual void onMouseWheel(int posX, int posY)
		{

		}

		glm::mat4 getView()
		{
			return glm::lookAt(_pos, _pos + _front, _up);
		}

		glm::mat4 getProjection()
		{
			return glm::perspective(glm::radians(_fov), (float)_width / _height, _znear, _zfar);
		}

#pragma region get set
		void setMoveSpeed(float speed)
		{
			_moveSpeed = speed;
			_moveSpeedOrg = speed;
		}

		float getMoveSpeed()
		{
			return _moveSpeed;
		}

		void setRotateSpeed(float speed)
		{
			_rotateSpeed = speed;
		}

		float getRotateSpeed()
		{
			return _rotateSpeed;
		}

		void setChangeTrans(bool change)
		{
			_changeTrans = change;
		}

		bool getChangeTrans()
		{
			return _changeTrans;
		}

		void setZfar(float zfar)
		{
			_zfar = zfar;
		}

		float getZfar()
		{
			return _zfar;
		}

		float getFOV()
		{
			return _fov;
		}

		glm::vec3 getPosition()
		{
			return _pos;
		}

		void setPosition(glm::vec3 pos)
		{
			_pos = pos;
		}

		glm::vec3 getFront()
		{
			return _front;
		}
#pragma endregion

	protected:
		float _moveSpeed = 4.0f;
		float _moveSpeedOrg = 4.0f;

		float _rotateSpeed = 0.25f;

		glm::vec3 _pos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 _front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f);

		int _width;
		int _height;
		float _fov = 45.0f;

		float _znear = 0.1f;
		float _zfar = 100.0f;

		// 标识摄像机变化
		bool _changeTrans = true;
	};
};

#endif // !__CameraBase_H__
