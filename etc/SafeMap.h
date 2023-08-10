#ifndef _SAFEMAP_H_
#define _SAFEMAP_H_

#include "./Config.h"

using std::iterator;

template <class Val>
class SafeMap
{
public:
	SafeMap();
	
	~SafeMap();

public:

	std::map<std::string, Val> safeMap;

private:

	//锁，多线程下，因为C++map是非线程安全的，所以当进行写的时候我们需要加上锁，在同一时间仅限一个线程进行删除和插入，查询不需要枷锁
	pthread_mutex_t SAFE_MAP_LOCK;   

public:

	void clear();

	void update(std::string& key, Val val);

	void insert(std::string key,Val val);

	void erase(std::string key);

	bool CheckoutIfExist(std::string key);

	typename std::map<std::string, Val>::iterator find(std::string key);

	typename std::map<std::string, Val>::iterator begin();

	typename std::map<std::string, Val>::iterator end();

	Val at(std::string key);

	Val& operator[](std::string& key);

	size_t size();

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
	try
	{
		pthread_mutex_lock(&SAFE_MAP_LOCK);		//上锁
		this->safeMap[key] = val;
		pthread_mutex_unlock(&SAFE_MAP_LOCK);	//解锁
	}
	catch (std::exception& err)
	{
		LOG.Log() << err.what() << std::endl;
	}
	catch (...)
	{
		LOG.Log() << "Some Err With Safe Insert Key : " << key << ", val :"<< &val << std::endl;
	}

	return;
}

template<class Val>
inline void SafeMap<Val>::erase(std::string key)
{
	try
	{
		pthread_mutex_lock(&SAFE_MAP_LOCK);		//上锁
		this->safeMap.erase(key);
		pthread_mutex_unlock(&SAFE_MAP_LOCK);	//解锁
	}
	catch (std::exception& err)
	{
		LOG.Log() << err.what() << std::endl;
	}
	catch (...)
	{
		LOG.Log() << "Some Err With Safe Erase Key : "<< key << std::endl;
	}
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
inline typename std::map<std::string, Val>::iterator SafeMap<Val>::find(std::string key)
{
	return this->safeMap.find(key);
}

template<class Val>
inline typename std::map<std::string, Val>::iterator SafeMap<Val>::begin()
{
	return this->safeMap.begin();
}

template<class Val>
inline typename std::map<std::string, Val>::iterator SafeMap<Val>::end()
{
	return this->safeMap.end();
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

template<class Val>
inline void SafeMap<Val>::clear()
{
	this->safeMap.clear();
	return;
}

template<class Val>
inline void SafeMap<Val>::update(std::string& key, Val val)
{
	this->safeMap[key] = val;
	return;
}

template<class Val>
inline size_t SafeMap<Val>::size()
{
	return this->safeMap.size();
}

