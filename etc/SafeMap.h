#ifndef _SAFEMAP_H_
#define _SAFEMAP_H_

#include "./Config.h"

template <class Val>
class SafeMap
{
public:
	SafeMap();
	
	~SafeMap();

private:
	std::map<std::string, Val> safeMap;

	//锁，多线程下，因为C++map是非线程安全的，所以当进行写的时候我们需要加上锁，在同一时间仅限一个线程进行删除和插入，查询不需要枷锁
	pthread_mutex_t SAFE_MAP_LOCK;   

public:
	void insert(std::string key,Val val);

	void erase(std::string key);

	bool CheckoutIfExist(std::string key);

	std::iterator<std::string, Val> find(std::string key);

	std::iterator<std::string, Val> begin();

	std::iterator<std::string, Val> end();

	Val at(std::string key);

	Val& operator[](std::string& key);
};

#endif

template<class Val>
inline SafeMap<Val>::SafeMap()
{
	this->safeMap.clear();

	pthread_mutex_init(&SAFE_MAP_LOCK, NULL);	//初始化锁
}

template<class Val>
inline SafeMap<Val>::~SafeMap()
{
}

template<class Val>
inline void SafeMap<Val>::insert(std::string key, Val val)
{
	pthread_mutex_lock(&SAFE_MAP_LOCK);		//上锁
	this->safeMap[key] = val;
	pthread_mutex_unlock(&SAFE_MAP_LOCK);	//解锁
	
	return;
}

template<class Val>
inline void SafeMap<Val>::erase(std::string key)
{
	pthread_mutex_lock(&SAFE_MAP_LOCK);		//上锁
	this->safeMap.erase(key);
	pthread_mutex_unlock(&SAFE_MAP_LOCK);	//解锁

	return;
}

template<class Val>
inline bool SafeMap<Val>::CheckoutIfExist(std::string key)
{
	if(this->safeMap.find(key) != this->safeMap.end())
	{
		return true;
	}

	return false;
}

template<class Val>
inline std::iterator<std::string, Val> SafeMap<Val>::find(std::string key)
{
	return std::iterator<std::string, Val>(this->safeMap.find(key));
}

template<class Val>
inline std::iterator<std::string, Val> SafeMap<Val>::begin()
{
	return std::iterator<std::string, Val>(this->safeMap.begin());
}

template<class Val>
inline std::iterator<std::string, Val> SafeMap<Val>::end()
{
	return std::iterator<std::string, Val>(this->safeMap.end());
}

template<class Val>
inline Val SafeMap<Val>::at(std::string key)
{
	return Val(this->safeMap.at(key));
}

template<class Val>
inline Val& SafeMap<Val>::operator[](std::string& key)
{
	return this->safeMap[key];
}

