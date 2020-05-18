#ifndef CMD_LIST_H
#define CMD_LIST_H

#include <string>

struct CmdList
{
public:
    std::string to_string(){
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
    auto size(){
        return commands.size();
    }
    auto empty(){
        return commands.empty();
    }
    auto emplace_back(std::string&& cmd){
        commands.emplace_back(cmd);
    }
    auto clear(){
        commands.clear();
    }
    std::vector<std::string>& get_data(){
        return commands;
    }
private:
    std::vector<std::string> commands;
};

#endif //CMD_LIST_H
