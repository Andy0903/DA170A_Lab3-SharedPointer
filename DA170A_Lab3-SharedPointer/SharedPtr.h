#pragma once
#include <stdexcept>
#include "ReferenceCounter.h"
#include "WeakPtr.h"

template<typename T>
class SharedPtr
{
	friend class WeakPtr<T>;

	T *ptr;
	ReferenceCounter *rc;

	void Remove()
	{
		if (rc != nullptr && rc->DecrementShared() == 0)
		{
			delete ptr;
			if (rc->GetWeak() == 0)
			{
				delete rc;
			}
		}
	}

public:

	SharedPtr() : ptr(nullptr), rc(nullptr)
	{
		rc = new ReferenceCounter();
		rc->IncrementShared();
	}

	SharedPtr(nullptr_t nptr) : SharedPtr()
	{
	}

	SharedPtr(T *ptr) : ptr(ptr), rc(nullptr)
	{
		rc = new ReferenceCounter();
		rc->IncrementShared();
	}

	SharedPtr(const SharedPtr<T>& sp) : ptr(sp.ptr), rc(sp.rc)
	{
		rc->IncrementShared();
	}

	SharedPtr(SharedPtr<T>&& sp) : ptr(nullptr), rc(nullptr)
	{
		ptr = sp.ptr;
		rc = sp.rc;

		sp.ptr = nullptr;
		sp.rc = nullptr;
	}

	SharedPtr(const WeakPtr<T> &wp) : ptr(wp.ptr), rc(wp.rc) {
		if (rc != nullptr)
			rc->IncrementShared();
	}

	~SharedPtr()
	{
		Remove();
	}

	bool Unique() const
	{
		return rc != nullptr && rc->GetShared() <= 1;
	}

	void Reset(T* ptr)
	{
		Remove();
		ptr = ptr;
		rc->IncrementShared();
	}

	void Reset()
	{
		Remove();
		ptr = nullptr;
		rc = nullptr;
	}

	SharedPtr<T>& operator=(const SharedPtr<T>& sp)
	{
		if (this != &sp)
		{
			if (rc != nullptr && rc->DecrementShared() == 0)
			{
				delete ptr;
				delete rc;
			}

			ptr = sp.ptr;
			rc = sp.rc;
			rc->IncrementShared();
		}

		return *this;
	}

	SharedPtr<T>& operator=(SharedPtr<T>&& sp)
	{
		if (this != &sp)
		{
			if (rc != nullptr && rc->DecrementShared() == 0)
			{
				delete ptr;
				delete rc;
			}

			ptr = sp.ptr;
			rc = sp.rc;
			rc->IncrementShared();
		}

		return *this;
	}

	operator bool() const
	{
		return ptr != nullptr;
	}

	T& operator*() const
	{
		return *ptr;
	}

	T* operator->() const
	{
		return ptr;
	}

	bool operator==(const SharedPtr<T>& rhs) const
	{
		return ptr == rhs.ptr;
	}

	bool operator<(const SharedPtr<T>& rhs) const
	{
		return ptr < rhs.ptr;
	}

	T* Get() const
	{
		return ptr;
	}
};

