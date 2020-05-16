#ifndef BULK_H
#define BULK_H
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <chrono>
#include <iostream>
#include "IObserver.h"
#include "IObservable.h"
class Bulk
{
public:
	Bulk(int size);
	~Bulk();
	void Process(std::string&& cmd);
	IObservable<std::string>& GetResultNotifier();
	IObservable<std::chrono::system_clock::time_point>& GetTimeNotifier();
private: //methods
	void NotifyResult();
	void NotifyTime();
	std::string PrepareOutput();
private:
	bool has_nested{ false };
	size_t bulk_size;
	int nested_counter{ 0 };
	IObservable<std::string> result_notifier;
	IObservable<std::chrono::system_clock::time_point> time_notifier;
	std::vector<std::string> commands;
};


#endif //BULK_H
