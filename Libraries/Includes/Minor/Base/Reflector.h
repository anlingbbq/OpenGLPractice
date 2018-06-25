#ifndef __Reflector_H__
#define __Reflector_H__

#include <iostream>
#include <string.h>
#include <map>

typedef void* (*PTRCreateObject)();

class Reflector
{
public:
	void* getClassByName(std::string name)
	{
		std::map<std::string, PTRCreateObject>::iterator iter = _map.find(name);
		if (iter == _map.end()) { return nullptr; }

		PTRCreateObject fun = iter->second;
		if (!fun) { return nullptr; }

		return fun();
	}

	void registClass(std::string name, PTRCreateObject fun)
	{
		_map[name] = fun;
	}

	static Reflector& Reflector::Instance()
	{
		static Reflector _reflector;
		return _reflector;
	}

private:
	Reflector() {};
	std::map<std::string, PTRCreateObject> _map;
};

class RegisterAction 
{
public:
	RegisterAction(std::string className, PTRCreateObject ptrCreateFn) {
		Reflector::Instance().registClass(className, ptrCreateFn);
	}
};

#define REGISTER(className)                                             \
    className* objectCreator##className(){                              \
        return new className;                                           \
    }                                                                   \
    RegisterAction g_creatorRegister##className(                        \
        #className,(PTRCreateObject)objectCreator##className)

#endif // !__Reflector_H__
