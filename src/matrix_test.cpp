#include "matrix.hpp"

#include <gtest/gtest.h>

#include <string>
#include <vector>

TEST(SimpleTest, TransposeSameSide) {
  std::vector<std::vector<int>> source = {
      {1, 2, 3},
      {4, 5, 6},
      {7, 8, 9},
  };

  std::vector<std::vector<int>> expected = {
      {1, 4, 7},
      {2, 5, 8},
      {3, 6, 9},
  };

  std::vector<std::vector<int>> output = matrix::transpose(source);

  for (size_t i = 0; i < source.size(); i++) {
    for (size_t j = 0; j < source.at(0).size(); j++) {
      int a = output[i][j];
      int b = expected[i][j];
      EXPECT_EQ(a, b);
    }
  }
}
