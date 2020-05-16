#ifndef FILE_LOGGER_H
#define FILE_LOGGER_H
#include <fstream>
#include "IObserver.h"

using timepoint = std::chrono::system_clock::time_point;
struct FileLogger : public IObserver<std::string>, public IObserver<timepoint>
{
    ~FileLogger(){};
	virtual void Update(std::string param) override {
		std::ofstream file(filename);
		if (file.is_open())
		{
			file << param;
			file.close();
		}
	}
	virtual void Update(timepoint time) override {
		filename = "bulk" + std::to_string(time.time_since_epoch().count()) + ".log";
	}
private:
	std::string filename;
};

#endif //FILE_LOGGER_H
