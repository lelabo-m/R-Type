#ifndef FACTORY_H_
#define FACTORY_H_

#include <map>
#include <string>

template <typename... T>
class IPrototype;

template <typename Object>
class IPrototype<Object>
{
public:
	virtual	Object*		Clone() const = 0;
	virtual	~IPrototype() {};
};

template <typename Object, typename... Parameter>
class IPrototype<Object, Parameter...>
{
public:
	virtual	Object*		Clone(Parameter...) const = 0;
	virtual	~IPrototype() {};
};

template <typename... T>
class Factory;

template <typename Key, typename Object>
class Factory<Key, Object>
{
public:
	Factory() {};
	~Factory() {};
	void		Learn(Key key, Object* obj)
	{
		_memory[key] = dynamic_cast<IPrototype<Object>*>(obj);
	}
	Object*		Create(Key key)
	{
		if (_memory.find(key) != _memory.end() && _memory[key] != nullptr)
			return _memory[key]->Clone();
		return nullptr;
	}
protected:
	std::map<Key, IPrototype<Object>*>	_memory;
};

template <typename Key, typename Object, typename... Parameter>
class Factory<Key, Object, Parameter...>
{
public:
	Factory() {};
	~Factory() {};
	void		Learn(Key key, Object* obj)
	{
		_memory[key] = dynamic_cast<IPrototype<Object, Parameter...>*>(obj);
	}
	Object*		Create(Key key, Parameter... params)
	{
		if (_memory.find(key) != _memory.end() && _memory[key] != nullptr)
			return _memory[key]->Clone(params...);
		return nullptr;
	}
private:
	std::map<Key, IPrototype<Object, Parameter...>*>	_memory;
};

#endif /* !FACTORY_H_ */