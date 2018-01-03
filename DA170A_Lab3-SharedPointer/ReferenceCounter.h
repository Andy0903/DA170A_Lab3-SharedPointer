#pragma once
class ReferenceCounter
{
private:
	int shared;
	int weak;

public:
	ReferenceCounter() : shared(0), weak(0) {}
	ReferenceCounter(int shared) : shared(shared), weak(0) {}
	ReferenceCounter(int shared, int weak) : shared(shared), weak(weak) {}

	void IncrementShared()
	{
		++shared;
	}

	int DecrementShared()
	{
		return --shared;
	}

	void IncrementWeak()
	{
		++weak;
	}

	int DecrementWeak()
	{
		return --weak;
	}

	int GetShared() const
	{
		return shared;
	}

	int GetWeak() const
	{
		return weak;
	}
};