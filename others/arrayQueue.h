#pragma once
#include "Queue.h"
#include <iostream>
using namespace std;

template<class T>
class arrayQueue : public Queue<T>
{	
	template<typename U>
	friend ostream& operator<<(ostream& out, const arrayQueue<U>& theQueue);
	template<typename U>
	friend istream& operator >> (istream& in, arrayQueue<U>& theQueue);
public:
	arrayQueue(int initialCapacity = 10);
	~arrayQueue();
	arrayQueue(const arrayQueue<T> & theQueue);
	arrayQueue<T>& operator=(const arrayQueue<T>& theQueue);

	bool empty() const;
	size_t size() const;
	T& front() const;
	T& back() const;
	void pop();
	void push(const T& theElement);

	//extend

	void split(arrayQueue<T>& lhs, arrayQueue<T>& rhs) const;
	void merge(const arrayQueue<T>& lhs, const arrayQueue<T>& rhs);
	

private:
	void changeArrayLength(size_t newLength);
	size_t arrayLength;
	int theFront;
	int theBack;
	T* element;
};

template<typename U>
ostream& operator<<(ostream& out, const arrayQueue<U>& theQueue)
{
	if (!theQueue.empty())
	{
		int i = (theQueue.theFront + 1) % theQueue.arrayLength;
		for (; i != theQueue.theBack; i++)
		{
			i %= theQueue.arrayLength;
			out << theQueue.element[i] << " ";
		}
		out << theQueue.element[theQueue.theBack];
	}
	return out;
}

template<typename U>
istream& operator >> (istream& in, arrayQueue<U>& theQueue)
{
	U theElement;
	while (in >> theElement)
	{
		theQueue.push(theElement);
	}
	return in;
}
template<class T>
arrayQueue<T>::arrayQueue(int initialCapacity):
	arrayLength(initialCapacity),
	theFront(initialCapacity-1),
	theBack(initialCapacity-1),
	element(new T[initialCapacity])
{}


template<class T>
arrayQueue<T>::~arrayQueue()
{
	delete element;
}

template<class T>
arrayQueue<T>::arrayQueue(const arrayQueue<T> & theQueue) :
	arrayLength(theQueue.arrayLength),
	theFront(theQueue.Front),
	theBack(theQueue.Back),
	element(new T[theQueue.arrayLength])
{
	copy(theQueue.element, theQueue.element + theQueue.arrayLength, element);
}


template<class T>
arrayQueue<T>& arrayQueue<T>::operator=(const arrayQueue<T> & theQueue)
{
	T* tempElement = new T[theQueue.arrayLength];
	copy(theQueue.element, theQueue.element + theQueue.arrayLength, tempElement);
	arrayLength = theQueue.arrayLength;
	theFront = theQueue.theFront;
	theBack = theQueue.theBack;
	delete element;
	element = tempElement;
	return *this;
}

template<class T>
bool arrayQueue<T>::empty() const
{
	return theFront == theBack;
}


template<class T>
size_t arrayQueue<T>::size() const
{
	if (theBack >= theFront)
	{
		return (size_t)(theBack - theFront);
	}
	else
	{
		return (size_t)(theBack + arrayLength - theFront);
	}
}

template<class T>
T& arrayQueue<T>::front() const
{
	return element[(theFront + 1) % arrayLength];
}

template<class T>
T& arrayQueue<T>::back() const
{
	return element[theBack];
}


template<class T>
void arrayQueue<T>::pop()
{
	theFront = (theFront + 1) % arrayLength;
	element[theFront].~T();
	
}

template<class T>
void arrayQueue<T>::push(const T& theElement)
{
	theBack = (theBack + 1) % arrayLength;
	if( theBack == theFront)
	{
		changeArrayLength(2 * arrayLength);
	}
	
	element[theBack] = theElement;
}

template<class T>
void arrayQueue<T>::changeArrayLength(size_t newLength)
{
	T* newQueue = new T[newLength];
	int start = (theFront + 1) % arrayLength;
	if (start < 2)
	{
		copy(element + start, element + arrayLength, newQueue);
	}
	else
	{
		copy(element + start, element + arrayLength, newQueue);
		copy(element, element + theBack + 1, newQueue + arrayLength - start);
	}

	theFront = newLength - 1;
	theBack = arrayLength - 1;
	arrayLength = newLength;
	delete element;
	element = newQueue;
}



template<class T>
void arrayQueue<T>::split(arrayQueue<T>& lhs, arrayQueue<T>& rhs) const
{
	for (int i = theFront + 1;; i++)
	{
		i = i % arrayLength;
		if (i % 2 == 1)
		{
			lhs.push(element[i]);
		}
		else
		{
			rhs.push(element[i]);
		}

		if (i == theBack)
		{
			break;
		}
	}
	
}

template<class T>
void arrayQueue<T>::merge(const arrayQueue<T>& lhs, const arrayQueue<T>& rhs)
{
	int lhsStart = (lhs.theFront + 1) % lhs.arrayLength, lhsEnd = (lhs.theBack + 1) % lhs.arrayLength;
	int rhsStart = (rhs.theFront + 1) % rhs.arrayLength, rhsEnd = (rhs.theBack + 1) % rhs.arrayLength;

	arrayQueue<T> * newQueue = new arrayQueue<T>;
	while (lhsStart != lhsEnd && rhsStart != rhsEnd)
	{
		newQueue->push(lhs.element[lhsStart]);
		newQueue->push(rhs.element[rhsStart]);
		lhsStart = (lhsStart + 1) % lhs.arrayLength;
		rhsStart = (rhsStart + 1) % rhs.arrayLength;
	}
	while (lhsStart != lhsEnd)
	{
		newQueue->push(lhs.element[lhsStart]);
		lhsStart = (lhsStart + 1) % lhs.arrayLength;
	}

	while (rhsStart != rhsEnd)
	{
		newQueue->push(rhs.element[rhsStart]);
		rhsStart = (rhsStart + 1) % rhs.arrayLength;
	}

	*this = *newQueue;
}