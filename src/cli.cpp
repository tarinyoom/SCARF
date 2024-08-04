#include "cli.hpp"

#include <span>
#include <functional>

#include "black_box.hpp"
#include "three_body.hpp"
#include "mov.hpp"

auto run_cli(std::ostream& out, std::vector<std::string>& args) -> int {
	if (args.size() <= 1) {
		return -1;
	}

	auto mode = args[0];
	auto anim = args[1];
    if (mode == "mov") {
		if (anim == "black_box") {
			return make_mov(black_box);
		} else {
			return make_mov(three_body);
		}
	} else if (mode == "gif") {
		throw "NYI";
	}
	return 0;
}
