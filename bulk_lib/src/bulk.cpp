#include "bulk.h"

Bulk::Bulk(int size) :bulk_size(size) {
}
Bulk::~Bulk() {
	if (!commands.empty() && !has_nested) {
		NotifyResult();
	}
}
void Bulk::NotifyResult() {
	result_notifier.Notify(PrepareOutput());
}
void Bulk::NotifyTime() {
	time_notifier.Notify(std::chrono::system_clock::now());
}
void Bulk::Process(std::string&& cmd) {
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
std::string Bulk::PrepareOutput() {
	std::string result{ "bulk: " };
	auto it = commands.begin();
	auto end = commands.end();
	for (; it != end; ++it) {
		if (it != commands.begin()) {
			result.append(", ");
		}
		result.append(*it);
	}
	return result;
}
IObservable<std::string>& Bulk::GetResultNotifier() {
	return result_notifier;
}
IObservable<std::chrono::system_clock::time_point>& Bulk::GetTimeNotifier() {
	return time_notifier;
}