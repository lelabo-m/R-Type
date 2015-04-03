#ifndef MEMORYPOOL_H_
#define MEMORYPOOL_H_

#include <list>

template <typename... T>
class IChunk;

template <typename Object>
class IChunk<Object>
{
public:
	virtual	void		Alloc() = 0;
	virtual	void		Free() = 0;
	virtual	~IChunk() {};
};

template <typename Object, typename... Parameter>
class IChunk<Object, Parameter...>
{
public:
	virtual	void		Alloc(Parameter...) = 0;
	virtual	void		Free() = 0;
	virtual	~IChunk() {};
};

template <typename... T>
class MemoryPool;

template <typename Object>
class MemoryPool<Object>
{
public:
	MemoryPool(size_t size)
	{
		_memory.clear();
		for (size_t i = 0; i < size; ++i)
			_memory.push_back(new Object());
	}
	~MemoryPool() { _memory.clear();  }
	Object*		Chunk()
	{
		if (!_memory.size())
			return new Object();
		Object*	freechunk = dynamic_cast<Object*>(_memory.front());
		_memory.pop_front();
		return freechunk;
	}
	void		Release(Object* obj)
	{
		obj->Free();
		_memory.push_back(obj);
	}
	void		Reserve(size_t size)
	{
		for (size_t i = 0; i < size; ++i)
			_memory.push_back(new Object());
	}
private:
	std::list<IChunk<Object>*>	_memory;
};

template <typename Object, typename... Parameter>
class MemoryPool<Object, Parameter...> 
{
public:
	MemoryPool(size_t size)
	{
		_memory.clear();
		for (size_t i = 0; i < size; ++i)
			_memory.push_back(new Object());
	}
	~MemoryPool() { _memory.clear(); }
	Object*		Chunk()
	{
		if (!_memory.size())
			return new Object();
		Object*	freechunk = dynamic_cast<Object*>(_memory.front());
		_memory.pop_front();
		return freechunk;
	}
	void		Release(Object* obj)
	{
		obj->Free();
		_memory.push_back(obj);
	}
	void		Reserve(size_t size)
	{
		for (size_t i = 0; i < size; ++i)
			_memory.push_back(new Object());
	}
private:
	std::list<IChunk<Object, Parameter...>*>	_memory;
};

#endif /* !MEMORYPOOL_H_ */