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

	//�������߳��£���ΪC++map�Ƿ��̰߳�ȫ�ģ����Ե�����д��ʱ��������Ҫ����������ͬһʱ�����һ���߳̽���ɾ���Ͳ��룬��ѯ����Ҫ����
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

	pthread_mutex_init(&SAFE_MAP_LOCK, NULL);	//��ʼ����
}

template<class Val>
inline SafeMap<Val>::~SafeMap()
{
}

template<class Val>
inline void SafeMap<Val>::insert(std::string key, Val val)
{
	pthread_mutex_lock(&SAFE_MAP_LOCK);		//����
	this->safeMap[key] = val;
	pthread_mutex_unlock(&SAFE_MAP_LOCK);	//����
	
	return;
}

template<class Val>
inline void SafeMap<Val>::erase(std::string key)
{
	pthread_mutex_lock(&SAFE_MAP_LOCK);		//����
	this->safeMap.erase(key);
	pthread_mutex_unlock(&SAFE_MAP_LOCK);	//����

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

