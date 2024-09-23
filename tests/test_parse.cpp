#include <gtest/gtest.h>

#include "parse/parse.hpp"

using namespace scarf;

TEST(parse, example_parse) {
  auto cfg = parse::parse_file("examples/config.txt");
  EXPECT_EQ(cfg.output_file.string(), "examples/generated.mov");
}
