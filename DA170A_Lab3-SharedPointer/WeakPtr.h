#pragma once

#include "ReferenceCounter.h"

template<typename T> class SharedPtr;

template<typename T>
class WeakPtr
{
	friend class SharedPtr<T>;

	T *ptr;
	ReferenceCounter *rc;

	void Remove()
	{
		if (rc != nullptr && rc->DecrementWeak() == 0 && rc->GetShared() == 0)
		{
			delete rc;
		}
	}

	bool ExpiredCheck()
	{
		if (rc == nullptr || rc->GetShared() == 0)
		{
			Remove();
			ptr = nullptr;
			rc = nullptr;
			return true;
		}

		return false;
	}


public:
	WeakPtr() : ptr(nullptr), rc(nullptr) {}
	WeakPtr(const nullptr_t &ptr) : ptr(nullptr), rc(nullptr) {}
	WeakPtr(const SharedPtr<T> &sp) : ptr(sp.ptr), rc(sp.rc)
	{
		if (rc != nullptr)
		{
			rc->IncrementWeak();
		}
	}

	WeakPtr(const WeakPtr<T> &wp) : ptr(wp.ptr), rc(wp.rc)
	{
		if (rc != nullptr)
		{
			rc->IncrementWeak();
		}
	}

	~WeakPtr()
	{
		Remove();
	}

	WeakPtr& operator=(const SharedPtr<T> rhs)
	{
		Remove();
		ptr = rhs.ptr;
		rc = rhs.rc;
		if (rc != nullptr)
		{
			rc->IncrementWeak();
		}
		return *this;
	}

	WeakPtr& operator=(const WeakPtr& rhs)
	{
		if (this != &rhs)
		{
			Remove();
			ptr = rhs.ptr;
			rc = rhs.rc;
			if (rc != nullptr)
			{
				rc->IncrementWeak();
			}
		}
		return *this;
	}

	T& operator*() 
	{
		ExpiredCheck();
		return (*ptr);
	}
	T* operator->() 
	{
		ExpiredCheck();
		return ptr;
	}

	operator bool() { return !ExpiredCheck(); }

	bool Expired()
	{
		return ExpiredCheck();
	}

	SharedPtr<T> Lock()
	{
		ExpiredCheck();
		return static_cast<SharedPtr<T>>(*this);
	}
};

