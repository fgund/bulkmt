#ifndef CONSOLE_LOGGER_H
#define CONSOLE_LOGGER_H
#include "IObserver.h"
#include <thread>
#include <atomic>
#include <queue>
#include <condition_variable>
#include <mutex>
#include "CmdList.h"

struct ConsoleLogger : public IObserver<CmdList>
{
    ConsoleLogger() {
        m_thread = std::thread(&ConsoleLogger::Run, this);
    }
    ~ConsoleLogger(){
        m_stopped = true;
        m_condition.notify_all();
        if(m_thread.joinable()) {
            m_thread.join();
        }
    };
	virtual void Update(CmdList param) override {
        m_tasks.push(param);
        m_condition.notify_one();
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
        while(!m_tasks.empty()) {
            Log(cmd_counter, block_counter);
        }
        //std::unique_lock<std::mutex> lck{m_mutex};
        std::cout << "Log_" << std::this_thread::get_id()
                  <<", blocks: " << block_counter
                  <<", commands: " << cmd_counter << std::endl;
    }
    void Log(size_t& cmd_counter, size_t& block_counter){
        auto param = m_tasks.front();
        m_tasks.pop();
        cmd_counter += param.size();
        ++block_counter;
        std::cout << param.to_string() << std::endl;
	}
private:
    std::condition_variable m_condition;
    std::thread m_thread;
    std::mutex m_mutex;
    std::queue<CmdList> m_tasks;
    std::atomic<bool> m_stopped{false};
};

#endif //CONSOLE_LOGGER_H
