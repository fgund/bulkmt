#ifndef TEST_LOGGER_H
#define TEST_LOGGER_H
#include <string>
#include <vector>
#include "IObserver.h"
#include "CmdList.h"
struct TestLogger : public IObserver<CmdList>
{
    ~TestLogger(){};
    virtual void Update(CmdList param) override {
        bulks.emplace_back(param.to_string());
    }
    const std::vector<std::string>& GetBulks(){
        return bulks.get_data();
    }
private:
    CmdList bulks;
};

#endif //TEST_LOGGER_H
