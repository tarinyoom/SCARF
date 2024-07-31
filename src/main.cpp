#include <string>
#include <vector>
#include <ostream>

#include "cli.hpp"
#include "mov.hpp"
#include "blackbox.hpp"

auto main(int argc, char* argv[]) -> int {
    std::vector<std::string> args;
    for (int i = 1; i < argc; i++) {
        args.push_back(std::string(argv[i]));
    }
    run_cli(std::cout, args);
}
