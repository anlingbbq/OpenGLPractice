#ifndef __MObject_H__
#define __MObject_H__

#include <iostream>
#include <glm/glm.hpp>
#include <Minor/Base/Reflector.h>

namespace minor
{
	class MObject
	{
	public:
		std::string name;

		virtual void destory()
		{

		}
	private:
	};
};

#endif // !__MObject_H__
