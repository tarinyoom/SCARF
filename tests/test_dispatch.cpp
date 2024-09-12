#include <gtest/gtest.h>

#include "alternator.hpp"

using namespace scarf;

TEST(dispatch, alternator) {
  auto alt = Alternator<double>(0.0, [](double n, double h) { return n + h; });
  EXPECT_EQ(*alt.next(1.0), 1.0);
  EXPECT_EQ(*alt.next(2.0), 3.0);
  EXPECT_EQ(*alt.next(4.0), 7.0);
  EXPECT_EQ(*alt.next(8.0), 15.0);
}
