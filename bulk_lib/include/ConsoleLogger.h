#ifndef CONSOLE_LOGGER_H
#define CONSOLE_LOGGER_H
#include "IObserver.h"
#include <thread>
#include <atomic>
#include <queue>
#include <condition_variable>
#include <mutex>

struct ConsoleLogger : public IObserver<std::string>
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
	virtual void Update(std::string param) override {
        m_tasks.push(param);
        m_condition.notify_one();
	}
private:
	void Run(){
	    while (!m_stopped) {
            std::unique_lock<std::mutex> lck{m_mutex};
            while (!m_stopped && m_tasks.empty())
                m_condition.wait(lck);

            if (m_stopped) {
                break;
            }
            auto param = m_tasks.front();
            m_tasks.pop();
            Log(param);
        }
        while(!m_tasks.empty()) {
            auto param = m_tasks.front();
            m_tasks.pop();
            Log(param);
        }
    }
    void Log(std::string_view param){
        std::cout << param.data() << std::endl;
	}
private:
    std::condition_variable m_condition;
    std::thread m_thread;
    std::mutex m_mutex;
    std::queue<std::string> m_tasks;
    std::atomic<bool> m_stopped{false};
};

#endif //CONSOLE_LOGGER_H
