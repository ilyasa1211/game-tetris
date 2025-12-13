#pragma once

#include <vector>
#include <raylib.h>

class Grid {
 private:
  std::vector<std::vector<bool>> grid_container;

 public:
  Grid(int width, int height);
  // returns how many rows that are erased
  const int EraseCompletedRows();
  void EraseRow(const int row) const;
  const void ShiftDownFloatingRows();
  void Draw(const int cell_size_px, const Color cell_color) const;
  int GetWidth() const;
  int GetHeight() const;

  std::vector<std::vector<bool>> Get() const;
  void Set(int x, int y, bool value);
};