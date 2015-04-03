template <typename... T> class Singleton;

template <class T> class Singleton<T>
{
public:		
		static T* GetInstance()
		{
			return instance;
		}
		static void Kill()
		{
			delete instance;
			instance = nullptr;
		}
		static void Init()
		{
			instance = nullptr;
		}
protected:
	static T* instance;
private:
	T& operator= (const T&){}
};
template <class T> T* Singleton<T>::instance = nullptr;