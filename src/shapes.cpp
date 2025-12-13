// 2d representation (height x width)
#include "shapes.hpp"

#include <raylib.h>

#include <array>
#include <memory>
#include <vector>

#include "grid.hpp"
#include "matrix.hpp"
#include "position.hpp"

Shape::Shape(int x, int y, Color color) : position(x, y), color(color) {};
Shape::Shape(int x, int y, Color color, std::vector<std::vector<bool>> shape) : position(x, y), color(color), shape(shape) {};

std::vector<std::vector<bool>> Shape::Get() const { return this->shape; };

std::unique_ptr<Shape> Shape::RotateShapeClockwise() const {
  return std::make_unique<Shape>(
      this->position.x, this->position.y, this->color,
      matrix::reflect_vertical(matrix::transpose(this->shape)));
};

std::unique_ptr<Shape> Shape::RotateShapeCounterclockwise() const {
  return std::make_unique<Shape>(
      this->position.x, this->position.y, this->color,
      matrix::reflect_horizontal(matrix::transpose(this->shape)));
};

bool Shape::IsTouchedOnTheNextXAxis(
    const Grid &grid,
    const int offset_x  // -1 (left) or 1 (right)
) const {
  for (int i = 0; i < this->shape.size(); i++) {
    for (int j = 0; j < this->shape.at(0).size(); j++) {
      if (!this->shape[i][j]) {
        continue;
      }

      if (grid.Get()
              .at(this->position.y + i)
              .at(this->position.x + j + offset_x)) {
        return true;
      }
    }
  }

  return false;
}

bool Shape::IsOnGround(const Grid &grid) const {
  for (int i = 0; i < this->shape.size(); i++) {
    for (int j = 0; j < this->shape.at(0).size(); j++) {
      if (!this->shape[i][j]) {
        continue;
      }

      // is on bottom
      if (this->position.y + i >= grid.Get().size()) {
        return true;
      }

      if (grid.Get().at(this->position.y + i).at(this->position.x + j)) {
        return true;
      }
    }
  }

  return false;
}

void Shape::Draw(const int cell_size_px) const {
  int y = this->position.y;

  for (const auto &row : this->shape) {
    int temp_x = this->position.x;

    for (const auto &column : row) {
      int col_x = temp_x++;

      if (!column) {
        continue;
      }

      DrawRectangle(static_cast<float>(col_x) * cell_size_px,
                    static_cast<float>(y) * cell_size_px,
                    cell_size_px, cell_size_px, this->color);
    }
    y++;
  }
};

std::unique_ptr<Shape> Shape::Clone() const { return std::make_unique<Shape>(*this); }

int Shape::GetWidth() const { return this->shape.at(0).size(); }

int Shape::GetHeight() const { return this->shape.size(); }

Position Shape::GetPosition() const { return this->position; }

void Shape::SetPositionX(const int &x) { this->position.x = x; }

void Shape::SetPositionY(const int &y) { this->position.y = y; }
