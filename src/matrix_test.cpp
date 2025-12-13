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

  for (int i = 0; i < expected.size(); i++) {
    for (int j = 0; j < expected.at(0).size(); j++) {
      int a = output[i][j];
      int b = expected[i][j];

      EXPECT_EQ(a, b);
    }
  }
}

TEST(SimpleTest, TransposeDifferentSide) {
  std::vector<std::vector<int>> source = {
      {1, 2, 3},
      {4, 5, 6},
  };

  std::vector<std::vector<int>> expected = {
      {1, 4},
      {2, 5},
      {3, 6},
  };

  std::vector<std::vector<int>> output = matrix::transpose(source);

  for (int i = 0; i < expected.size(); i++) {
    for (int j = 0; j < expected.at(0).size(); j++) {
      int a = output[i][j];
      int b = expected[i][j];

      EXPECT_EQ(a, b);
    }
  }
}

TEST(SimpleTest, ReflectHorizontalSameSide) {
  std::vector<std::vector<int>> src = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9},
  };

  std::vector<std::vector<int>> expected = {
    {7, 8, 9},
    {4, 5, 6},
    {1, 2, 3},
  };

  auto result = matrix::reflect_horizontal(src);

  for (int i = 0; i < expected.size(); i++)
  {
    for (int j = 0; j < expected.at(0).size(); j++)
    {
      EXPECT_EQ(result[i][j], expected[i][j]);
    }
  }
}

TEST(SimpleTest, ReflectHorizontalDifferentSide) {
  std::vector<std::vector<int>> src = {
    {1, 2, 3},
    {4, 5, 6},
  };

  std::vector<std::vector<int>> expected = {
    {4, 5, 6},
    {1, 2, 3},
  };

  auto result = matrix::reflect_horizontal(src);

  for (int i = 0; i < expected.size(); i++)
  {
    for (int j = 0; j < expected.at(0).size(); j++)
    {
      EXPECT_EQ(result[i][j], expected[i][j]);
    }
  }
}

TEST(SimpleTest, ReflectVerticalSameSide) {
  std::vector<std::vector<int>> src = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9},
  };

  std::vector<std::vector<int>> expected = {
    {3, 2, 1},
    {6, 5, 4},
    {9, 8, 7},
  };

  auto result = matrix::reflect_vertical(src);

  for (int i = 0; i < expected.size(); i++)
  {
    for (int j = 0; j < expected.at(0).size(); j++)
    {
      EXPECT_EQ(result[i][j], expected[i][j]);
    }
  }
}

TEST(SimpleTest, ReflectVerticalDifferentSide) {
  std::vector<std::vector<int>> src = {
    {1, 2},
    {4, 5},
    {7, 8},
  };

  std::vector<std::vector<int>> expected = {
    {2, 1},
    {5, 4},
    {8, 7},
  };

  auto result = matrix::reflect_vertical(src);

  for (int i = 0; i < expected.size(); i++)
  {
    for (int j = 0; j < expected.at(0).size(); j++)
    {
      EXPECT_EQ(result[i][j], expected[i][j]);
    }
  }
}