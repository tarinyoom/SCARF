#include <gtest/gtest.h>

#include "sph.hpp"

using namespace scarf;

TEST(scarf, end_to_end) {
  char arg1[] = "";
  char arg2[] = "examples/config.txt";
  char *args[] = {arg1, arg2};
  run(2, args);
}
