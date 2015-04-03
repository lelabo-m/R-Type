#ifndef OBSERVER_H_
#define OBSERVER_H_

template <typename... T>
class Observer;

template <typename Class, typename ClassMethod>
class Observer<Class, ClassMethod>
{
public:
	virtual void	OnEvent(ClassMethod ptr)
	{
		if (dynamic_cast<Class*>(this))
			(dynamic_cast<Class*>(this)->*ptr)();
	}
	virtual ~Observer() {};
};

template <typename Class, typename ClassMethod, typename... Param>
class Observer<Class, ClassMethod, Param...>
{
public:
	virtual void	OnEvent(ClassMethod ptr, Param... p)
	{
		if (dynamic_cast<Class*>(this))
			(dynamic_cast<Class*>(this)->*ptr)(p...);
	}
	virtual ~Observer() {};
};

template <typename... T>
class Subject;

template <typename Class, typename ClassMethod>
class Subject<Class, ClassMethod>
{
public:
	virtual	void	Register(Observer<Class, ClassMethod>* ptr, ClassMethod func)
	{
		_ptr = ptr;
		_func = func;
	}
	virtual void	Notify()
	{
		if (_ptr)
			_ptr->OnEvent(_func);
	}
	virtual void	Unregister()
	{
		_ptr = nullptr;
		_func = nullptr;
	}
	virtual ~Subject() {};
private:
	ClassMethod						_func;
	Observer<Class, ClassMethod>*	_ptr;
};

template <typename Class, typename ClassMethod, typename... Param>
class Subject<Class, ClassMethod, Param...>
{
public:
	virtual	void	Register(Observer<Class, ClassMethod, Param...>* ptr, ClassMethod func)
	{
		_ptr = ptr;
		_func = func;
	}
	virtual void	Notify(Param...	p)
	{
		if (_ptr)
			_ptr->OnEvent(_func, p...);
	}
	virtual void	Unregister()
	{
		_ptr = nullptr;
		_func = nullptr;
	}
	virtual ~Subject() {};
private:
	ClassMethod								_func;
	Observer<Class, ClassMethod, Param...>*	_ptr;
};



#endif /* !OBSERVER_H_ */