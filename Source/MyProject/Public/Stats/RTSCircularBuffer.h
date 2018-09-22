#pragma once
#include "Runtime/Core/Public/Containers/CircularBuffer.h"

template<typename T>
class RTSCircularBuffer
{
	uint32 currentIndex = TNumericLimits<uint32>::Max();
	TCircularBuffer<T> buffer;

public:

	RTSCircularBuffer(int capacity) : buffer(TCircularBuffer<T>(capacity))
	{
	
	}

	void Insert(T data)
	{
		buffer[buffer.GetNextIndex(currentIndex)] = T(data);
		++currentIndex;
	}

	/**Gets an item in the buffer a certain index away from the current one.  Not specifying an offset returns the current item*/
	T GetPrevItem(uint32 offset = 0)
	{
		return buffer[currentIndex - offset];
	}

	uint32 Size()
	{
		return buffer.Capacity();
	}
};