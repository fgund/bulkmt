#ifndef FILE_LOGGER_H
#define FILE_LOGGER_H
#include <fstream>
#include <thread>
#include <atomic>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <array>
#include "IObserver.h"
#include "CmdList.h"

using timepoint = std::chrono::system_clock::time_point;
struct FileLogger : public IObserver<CmdList>, public IObserver<timepoint>
{
    FileLogger(){
        for(auto& thread : m_thread_pool) {
            thread = std::thread(&FileLogger::Run, this);
        }
    }
    ~FileLogger(){
        m_stopped = true;
        m_condition.notify_all();
        for(auto& thread : m_thread_pool) {
            if(thread.joinable()) {
                thread.join();
            }
        }
    }
	virtual void Update(CmdList param) override {
        m_tasks.push(param);
        m_condition.notify_one();
	}
	virtual void Update(timepoint time) override {
		filename = "bulk" + std::to_string(time.time_since_epoch().count()) + ".log";
	}
private:
    void Run(){
        thread_local size_t cmd_counter{0};
        thread_local size_t block_counter{0};
        while (!m_stopped) {
            std::unique_lock<std::mutex> lck{m_mutex};
            while (!m_stopped && m_tasks.empty())
                m_condition.wait(lck);

            if (m_stopped) {
                break;
            }
            Log(cmd_counter, block_counter);
        }
        std::unique_lock<std::mutex> lck{m_mutex};
        while(!m_tasks.empty()) {
            Log(cmd_counter, block_counter);
        }
        std::cout << "File_" << std::this_thread::get_id()
                  <<", blocks: " << block_counter
                  <<", commands: " << cmd_counter << std::endl;
    }
    void Log(size_t& cmd_counter, size_t& block_counter){
        auto param = m_tasks.front();
        m_tasks.pop();
        cmd_counter += param.size();
        ++block_counter;
        std::ofstream file(filename);
        if (file.is_open())
        {
            file << std::this_thread::get_id()<< ":\n" << param.to_string();
            file.close();
        }
    }
private:
	std::string filename;
    std::condition_variable m_condition;
    std::array<std::thread,2> m_thread_pool;
    std::mutex m_mutex;
    std::queue<CmdList> m_tasks;
    std::atomic<bool> m_stopped{false};
};

#endif //FILE_LOGGER_H
