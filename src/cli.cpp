#include "cli.hpp"

#include <span>
#include <functional>

#include "black_box.hpp"
#include "mov.hpp"

auto run_cli(std::ostream& out, std::vector<std::string>& args) -> int {
	if (args.size() <= 0) {
		return -1;
	}

	auto mode = args[0];
    if (mode == "mov") {
		return make_mov(black_box);
	} else if (mode == "gif") {
		throw "NYI";
	}
	return 0;
}
