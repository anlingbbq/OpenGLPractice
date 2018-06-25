#ifndef __GameObject_H__
#define __GameObject_H__

#include <list>
#include <Minor/Base/check_type.hpp>
#include <Minor/Base/MObject.h>
#include <Minor/Base/MComponent.h>

namespace minor
{
	class GameObject : public MObject
	{
	public:
		GameObject()
		{
			_componentList = new std::list<MComponent*>();
		}

		template <typename T>
		std::string GetTypeName()
		{
			std::string typeName = check_type<T>();
			//std::cout << typeName << std::endl;

			return typeName.substr(typeName.find("::") + 2);
		}

		template <typename T>
		T* AddComponent()
		{
			return (T*)AddComponent(GetTypeName<T>());
		}

		MComponent* AddComponent(std::string name)
		{
			MComponent* component = (MComponent*)Reflector::Instance().getClassByName(name);
			AddComponent(component);
			return component;
		}

		void AddComponent(MComponent* component)
		{
			_componentList->push_back(component);
			component->gameObject = this;
		}

		void RemoveComponent(MComponent* component)
		{
			_componentList->remove(component);
		}

		template<typename T>
		T* GetComponent()
		{
			return GetComponent<T>(GetTypeName<T>());
		}

		template<typename T>
		T* GetComponent(std::string name)
		{
			//std::cout << name << std::endl;
			std::list<MComponent*>::iterator iter = _componentList->begin();
			for (; iter != _componentList->end(); iter++)
			{
				if (name == (*iter)->name)
					return (T*)*iter;
			}

			return nullptr;
		}

		template<typename T>
		T* AddMissingComponent()
		{
			MComponent* result = GetComponent(GetTypeName<T>());
			if (result == nullptr)
			{
				MComponent* component = (MComponent*)Reflector::Instance().getClassByName(name);
				_componentList->push_back(component);
				return component;
			}

			return result;
		}

		virtual void destory()
		{
			std::list<MComponent*>::iterator iter = _componentList->begin();
			for (; iter != _componentList->end(); iter++)
			{
				(*iter)->destory();
			}
			_componentList->clear();
			delete _componentList;
		}

	public:
		std::string tag;

	private:
		std::list<MComponent*>* _componentList;
	};
};

#endif // !__GameObject_H__
