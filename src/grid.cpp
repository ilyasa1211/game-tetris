#include "grid.hpp"

#include <raylib.h>

#include <vector>

#include "position.hpp"
#include "size.hpp"

Grid::Grid(int width, int height)
    : grid_container(height, std::vector<bool>(width, false)) {};

const int Grid::EraseCompletedRows() {
  int erased_rows = 0;

  for (int i = this->grid_container.size() - 1; i >= 0; i--) {
    int block_count = 0;
    for (auto &&col : this->grid_container.at(i)) {
      if (!col) {
        break;
      }
      block_count++;
    }

    if (block_count == this->grid_container.at(i).size()) {
      this->EraseRow(i);
      erased_rows++;
    }
  }

  return erased_rows;
};

void Grid::EraseRow(const int row) {
  for (auto &&col : this->grid_container.at(row)) {
    col = 0;
  }
};

const void Grid::ShiftDownFloatingRows() {
  for (int col = 0; col < this->GetWidth(); col++) {
    int offset = 0;

    for (int row = this->GetHeight() - 1; row >= 1; row--) {
      bool val = this->grid_container.at(row).at(col);

      if (!val) {
        offset++;
        continue;
      }

      this->Set(col, row + offset, 1);
      this->Set(col, row, 0);
    }
  }
};

void Grid::Draw(const int cell_size_px, const Color cell_color) const {
  for (int i = 0; i < this->grid_container.size(); i++) {
    for (int j = 0; j < this->grid_container.at(i).size(); j++) {
      if (this->grid_container[i][j]) {
        DrawRectangle(static_cast<float>(j) * cell_size_px,
                      static_cast<float>(i) * cell_size_px, cell_size_px,
                      cell_size_px, cell_color);
      }
    }
  }
};

int Grid::GetWidth() const { return this->grid_container.at(0).size(); };

int Grid::GetHeight() const { return this->grid_container.size(); };

std::vector<std::vector<bool>> Grid::Get() const {
  return this->grid_container;
};

void Grid::Set(int x, int y, bool value) {
  this->grid_container.at(y).at(x) = value;
};