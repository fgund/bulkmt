#include "bulk.h"

Bulk::Bulk(int size) :bulk_size(size) {
}
Bulk::~Bulk() {
	if (!commands.empty() && !has_nested) {
		NotifyResult();
	}
    std::cout << "Main. Lines: " << line_counter
              <<", blocks: " << block_counter
              <<", commands: " << cmd_counter << std::endl;
}
void Bulk::NotifyResult() {
    cmd_counter += commands.size();
    ++block_counter;
	result_notifier.Notify(commands);
}
void Bulk::NotifyTime() {
	time_notifier.Notify(std::chrono::system_clock::now());
}
void Bulk::Process(std::string&& cmd) {
    ++line_counter;
	if (cmd == "{") {
		has_nested = true;
		++nested_counter;
	}
	else if (cmd == "}") {
        if(nested_counter > 0){
            --nested_counter;
        }
    }
	else {
		if (commands.empty()) {
			NotifyTime();
		}
		commands.emplace_back(std::move(cmd));
	}
	if ((commands.size() == bulk_size) && (!has_nested)) {
		NotifyResult();
		commands.clear();
	}
	else if (has_nested && (nested_counter == 0)) {
		NotifyResult();
		has_nested = false;
		commands.clear();
	}
}

IObservable<CmdList>& Bulk::GetResultNotifier() {
	return result_notifier;
}
IObservable<std::chrono::system_clock::time_point>& Bulk::GetTimeNotifier() {
	return time_notifier;
}