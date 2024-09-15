#include <gtest/gtest.h>

#include "parser/parse.hpp"

using namespace scarf;

TEST(parser, example_parse) {
  auto cfg = parser::parse_file("examples/config.txt");
  EXPECT_EQ(cfg.output_file.string(), "examples/generated.mov");
}
