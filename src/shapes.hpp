#pragma once

#include <raylib.h>

#include <memory>
#include <vector>

#include "grid.hpp"
#include "position.hpp"

class Shape {
 protected:
  std::vector<std::vector<bool>> shape;
  Position position{0, 0};
  const Color color{255, 255, 255, 255};

 public:
  Shape(int x, int y, Color color);
  Shape(int x, int y, Color color, std::vector<std::vector<bool>> shape);

  std::unique_ptr<Shape> RotateShapeClockwise() const;
  std::unique_ptr<Shape> RotateShapeCounterclockwise() const;

  std::vector<std::vector<bool>> Get() const;

  bool IsTouchedOnTheNextXAxis(const Grid &grid,
                               const int offset_x  // -1 (left) or 1 (right)
  ) const;

  bool IsOnGround(const Grid &grid) const;
  void Draw(const int cell_size_px) const;

  std::unique_ptr<Shape> Clone() const;

  int GetWidth() const;
  int GetHeight() const;
  Position GetPosition() const;
  void SetPositionX(const int &x);
  void SetPositionY(const int &y);
};

class IShape : public Shape {
 public:
  IShape(int x, int y, Color color) : Shape(x, y, color) {
    this->shape = {
        {1},
        {1},
        {1},
        {1},
    };
  };
  IShape(int x, int y, Color color, std::vector<std::vector<bool>> shape) : Shape(x, y, color, shape) {};
};

class JShape : public Shape {
 public:
  JShape(int x, int y, Color color) : Shape(x, y, color) {
    this->shape = {
        {0, 1},
        {0, 1},
        {0, 1},
        {1, 1},
    };
  };
  JShape(int x, int y, Color color, std::vector<std::vector<bool>> shape) : Shape(x, y, color, shape) {};
};

class LShape : public Shape {
 public:
  LShape(int x, int y, Color color) : Shape(x, y, color) {
    this->shape = {
        {1, 0},
        {1, 0},
        {1, 0},
        {1, 1},
    };
  };
  LShape(int x, int y, Color color, std::vector<std::vector<bool>> shape) : Shape(x, y, color, shape) {};
};

class OShape : public Shape {
 public:
  OShape(int x, int y, Color color) : Shape(x, y, color) {
    this->shape = {
        {1, 1},
        {1, 1},
    };
  };
  OShape(int x, int y, Color color, std::vector<std::vector<bool>> shape) : Shape(x, y, color, shape) {};
};

class SShape : public Shape {
 public:
  SShape(int x, int y, Color color) : Shape(x, y, color) {
    this->shape = {
        {0, 1, 1},
        {1, 1, 0},
    };
  };
  SShape(int x, int y, Color color, std::vector<std::vector<bool>> shape) : Shape(x, y, color, shape) {};
};

class TShape : public Shape {
 public:
  TShape(int x, int y, Color color) : Shape(x, y, color) {
    this->shape = {
        {1, 1, 1},
        {0, 1, 0},
    };
  };
  TShape(int x, int y, Color color, std::vector<std::vector<bool>> shape) : Shape(x, y, color, shape) {};
};

class ZShape : public Shape {
 public:
  ZShape(int x, int y, Color color) : Shape(x, y, color) {
    this->shape = {
        {1, 1, 0},
        {0, 1, 1},
    };
  };
  ZShape(int x, int y, Color color, std::vector<std::vector<bool>> shape) : Shape(x, y, color, shape) {};
};