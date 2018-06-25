#ifndef __IComponent_H__
#define __IComponent_H__

#include <Minor/Base/MObject.h>

namespace minor
{
	class GameObject;
	class MComponent : public MObject
	{
	public:
		GameObject* gameObject;

		template <typename T>
		T* GetComponent()
		{
			return gameObject->GetComponent<T>();
		}

		virtual void destory()
		{
			gameObject = nullptr;
			delete this;
		}
	private:
	};
};

#endif // !__IComponent_H__
