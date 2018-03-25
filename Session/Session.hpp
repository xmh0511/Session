#pragma once
#include <iostream>
#include <string>
#include <any>
#include <map>
#include <chrono>
#include <ctime>
#include <mutex>
class Session;
std::map<std::string, Session*> *GLOBAL_SESSION=nullptr;
class Session
{
public:
	Session(std::string name, std::string id, std::size_t expire, std::string path = "", std::string domain = "")
	{
		this->name = name;
		this->id = id;
		this->expire = expire;
		this->path = path;
		this->domain = domain;
		std::time_t time = getTimeStamp();
		this->timestamp = expire * 1000 + time;
		if (GLOBAL_SESSION == nullptr) {
			Session::_threadLock.lock();
			GLOBAL_SESSION = new std::map<std::string, Session*>();
			Session::_threadLock.unlock();
		}
		Session::_threadLock.lock();
		GLOBAL_SESSION->insert(std::make_pair(this->id, this));
		Session::_threadLock.unlock();
	}
	void setData(std::string name, std::any data)
	{
		Session::_threadLock.lock();
		_data[name] = data;
		Session::_threadLock.unlock();
	}
	template<typename Type>
	Type getData(std::string name)
	{
		auto itert = _data.find(name);
		if (itert != _data.end())
		{
			return std::any_cast<Type>(itert->second);
		}
		return Type{};
	}
public:
	static Session* get(std::string id)
	{
		if (GLOBAL_SESSION != nullptr) 
		{
			auto iter = GLOBAL_SESSION->find(id);
			if (iter != GLOBAL_SESSION->end())
			{
				return iter->second;
			}
			return nullptr;
		}
		return nullptr;
	}
	static std::map<std::string, Session*>::iterator del(std::map<std::string, Session*>::iterator iter)
	{
			return GLOBAL_SESSION->erase(iter);
	}
	static void ticktime()
	{
		Session::_threadLock.lock();
		if (GLOBAL_SESSION != nullptr)
		{
			std::cout << "GLOBALSize=="<<GLOBAL_SESSION->size() << std::endl;
			std::time_t nowTimeStamp = Session::getTimeStamp();
			for (auto iter = GLOBAL_SESSION->begin(); iter != GLOBAL_SESSION->end();)
			{
				if (iter->second->timestamp < nowTimeStamp)
				{
					iter = Session::del(iter);
				}
				else {
					iter++;
				}
			}
		}
		Session::_threadLock.unlock();
	}
public:
	static std::mutex _threadLock;
private:
	std::string name;
	std::string id;
	std::string path;
	std::string domain;
	std::size_t expire;
	std::time_t timestamp;
	std::map<std::string, std::any> _data;
private:
	Session()
	{

	}
public:
	static std::time_t getTimeStamp()
	{
		std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
		auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
		std::time_t timestamp = tmp.count();
		return timestamp;
	}
};
std::mutex Session::_threadLock;