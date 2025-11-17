
#pragma once

#include <vector>

namespace matrix {

template <typename T>
std::vector<std::vector<T>> transpose(std::vector<std::vector<T>> src) {
  int row = src.size();
  int col = src.at(0).size();

  std::vector<std::vector<T>> out(col, std::vector<T>(row));

  for (size_t i = 0; i < row; i++) {
    for (size_t j = 0; j < col; j++) {
      out[j][i] = src[i][j];
    }
  }

  return out;
}
}  // namespace matrix