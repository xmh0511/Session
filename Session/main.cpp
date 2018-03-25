#include "Session.hpp"
#include <thread>
void _thread()
{
	while (true)
	{
		Session::ticktime();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		auto s = Session::get("abc123456");
		if (s != nullptr)
		{
			std::cout << s->getData<int>("abc") << std::endl;
		}
		else {
			std::cout << "earse" << std::endl;
		}
	}
}
void _thread2()
{
	while (true)
	{
		Session::ticktime();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		auto s = Session::get("jkf");
		if (s != nullptr)
		{
			std::cout <<"thread2=="<< s->getData<int>("abc") << std::endl;
		}
		else {
			std::cout << "thread2==" << "earse" << std::endl;
		}
	}
}
void _thread3()
{
	static int cout = 0;
	while ((cout++) < 50)
	{
		Session *ss = new Session("SessionId", "abc"+std::to_string(Session::getTimeStamp()), 30);
		std::cout << "create==="<< cout << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}
int main()
{
	Session ss("SessionId","abc123456",10);
	ss.setData("abc", 10);
	Session ss2("SessionId", "jkf", 20);
	ss2.setData("abc", 30);
	std::thread t1(&_thread);
	t1.detach();
	std::thread t2(&_thread2);
	t2.detach();
	std::thread t3(&_thread3);
	t3.detach();
	std::cin.get();
	return 0;
}