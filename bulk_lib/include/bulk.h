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
#include "CmdList.h"

class Bulk
{
public:
	Bulk(int size);
	~Bulk();
	void Process(std::string&& cmd);
	IObservable<CmdList>& GetResultNotifier();
	IObservable<std::chrono::system_clock::time_point>& GetTimeNotifier();
private: //methods
	void NotifyResult();
	void NotifyTime();
private:
    size_t cmd_counter{0};
    size_t block_counter{0};
    size_t line_counter{0};
	bool has_nested{ false };
	size_t bulk_size;
	int nested_counter{ 0 };
	IObservable<CmdList> result_notifier;
	IObservable<std::chrono::system_clock::time_point> time_notifier;
    CmdList commands;
};


#endif //BULK_H
