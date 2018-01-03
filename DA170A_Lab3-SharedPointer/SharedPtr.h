#pragma once

template<typename T>
class SharedPtr
{
	class ReferenceCounter
	{
	private:
		int count;

	public:
		ReferenceCounter() : count(0)
		{
		}

		void Increment()
		{
			++count;
		}

		int Decrement()
		{
			return --count;
		}

		int GetCount() const
		{
			return count;
		}
	};

public:
	T *data;
	ReferenceCounter *rc;

	SharedPtr() : data(nullptr), rc(nullptr)
	{
		rc = new ReferenceCounter();
		rc->Increment();
	}

	SharedPtr(nullptr_t nptr) : SharedPtr()
	{
	}

	SharedPtr(T *ptr) : data(ptr), rc(nullptr)
	{
		rc = new ReferenceCounter();
		rc->Increment();
	}

	SharedPtr(const SharedPtr<T>& sp) : data(sp.data), rc(sp.rc)
	{
		rc->Increment();
	}

	SharedPtr(SharedPtr<T>&& sp) : data(nullptr), rc(nullptr)
	{
		data = sp.data;
		rc = sp.rc;

		sp.data = nullptr;
		sp.rc = nullptr;
	}

	~SharedPtr()
	{
		if (rc != nullptr && rc->Decrement() == 0)		//TODO varför nullcheck? out of scope crash i G. Något fel på reset?
		{
			delete data;
			delete rc;
		}
	}

	bool Unique() const
	{
		return !(rc->GetCount() > 1);
	}

	void Reset(T* ptr)
	{
		if (rc->Decrement == 0)
		{
			delete data;
		}

		data = ptr;
		rc->Increment();
	}

	void Reset()
	{
		if (rc->Decrement() == 0)
		{
			delete data;
			delete rc;

		}

		data = nullptr;
	}

	SharedPtr<T>& operator=(const SharedPtr<T>& sp)
	{
		if (this != &sp)
		{
			if (rc->Decrement() == 0)
			{
				delete data;
				delete rc;
			}

			data = sp.data;
			rc = sp.rc;
			rc->Increment();
		}

		return *this;
	}

	SharedPtr<T>& operator=(SharedPtr<T>&& sp)
	{
		if (this != &sp)
		{
			if (rc->Decrement() == 0)
			{
				delete data;
				delete rc;
			}

			data = sp.data;
			rc = sp.rc;
			rc->Increment();
		}

		return *this;
	}

	operator bool() const
	{
		return data != nullptr;
	}

	T& operator*() const
	{
		return *data;
	}

	T* operator->() const
	{
		return data;
	}

	bool operator==(const SharedPtr<T>& rhs) const
	{
		return data == rhs.data;
	}

	bool operator<(const SharedPtr<T>& rhs) const
	{
		return data < rhs.data;
	}

	T* Get() const
	{
		return data;
	}
};

