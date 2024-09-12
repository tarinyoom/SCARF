#include <gtest/gtest.h>

#include "alternator.hpp"

using namespace scarf;

TEST(dispatch, alternator) {
  auto alt = Alternator<int>(0, [](int i) { return i + 1; });
  EXPECT_EQ(*alt.next(), 1);
  EXPECT_EQ(*alt.next(), 2);
  EXPECT_EQ(*alt.next(), 3);
  EXPECT_EQ(*alt.next(), 4);
}
