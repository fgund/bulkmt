#include <iostream>
#include "boost/program_options.hpp"
#include "bulk.h"
#include "ConsoleLogger.h"
#include "FileLogger.h"

namespace po = boost::program_options;

int main(int argc, char *argv[])
{

    int bulk_size{0};
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
                ("help", "produce help message")
                ("N", po::value<int>(&bulk_size), "set the bulk size");
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return 1;
        }

        if (vm.count("N")) {
            std::cout << "Bulk size was set to "
                      << vm["N"].as<int>() << ".\n";
        } else {
            std::cout << "Bulk size was not set.\n";
            std::cout << desc << "\n";
            return 1;
        }
    }
    catch(std::exception& e){
        std::cerr << e.what() << std::endl;
        return 1;
    }

    if (bulk_size <= 0) {
        std::cout << "Bulk size should be positive.\n";
        return 1;
    }
    Bulk bulk_unit(bulk_size);
    auto console_logger = std::make_shared<ConsoleLogger>();
    auto file_logger = std::make_shared<FileLogger>();
    bulk_unit.GetResultNotifier().AddObserver(console_logger);
    bulk_unit.GetResultNotifier().AddObserver(file_logger);
    bulk_unit.GetTimeNotifier().AddObserver(file_logger);
    std::string input;
    while (true) {
        std::getline(std::cin, input);
        if (std::cin.eof()) {
            break;
        }
        bulk_unit.Process(std::move(input));
    }

	return 0;
}