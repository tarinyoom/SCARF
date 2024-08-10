#include "cli.hpp"

#include <span>
#include <functional>

#include "three_body.hpp"
#include "mov.hpp"

auto run_cli(std::ostream& out, std::vector<std::string>& args) -> int {
	if (args.size() == 0 || args[0] == "three_body") {
		return make_mov(three_body);
	} else {
		return 0;
	}
}
