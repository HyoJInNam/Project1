#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include "./stdafx.h"

template<typename T>
class SINGLETON
{
protected:
	static T* instance;

	SINGLETON() {};
	SINGLETON(const SINGLETON& other) {};
	~SINGLETON() {};

	static void destroy() { SAFE_DELETE(instance); };

public:
	static T* GetInstance()
	{
		if (instance==nullptr) {
			instance = new T;
			atexit(destroy);
		}
		return instance;
	};
};


template <typename T> T* SINGLETON <T>::instance = nullptr;

#endif