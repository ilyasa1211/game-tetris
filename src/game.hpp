#pragma once

#include <array>

#include "action.hpp"
#include "grid.hpp"
#include "input.hpp"
#include "position.hpp"
#include "shapes.hpp"
#include "size.hpp"

class Game {
 private:
  const float move_interval_seconds = 0.5f;
  const float auto_score_interval_seconds =
      5.f;  // auto get score every 5 seconds

  float accumulated_time_seconds = 0.f;
  float accumulated_auto_score_seconds = 0.f;
  float time_played_seconds = 0.f;

  int scores = 0;
  const int score_multiplier = 10;
  const int cell_size_px = 20;
  ACTION action = ACTION::NONE;

  const Input &input;
  Grid grid;

  // color
  const Color shape_color_rgba{.r = 200, .g = 100, .b = 0, .a = 255};
  const Color cell_color_rgba{.r = 100, .g = 200, .b = 150, .a = 255};
  const Color placeholder_color_rgba{.r = 100, .g = 200, .b = 150, .a = 255};

  // shape spawn area width from the top of screen
  const std::array<int, 2> spawn_area_range{5, 15};

  // first and second shape
  std::array<std::unique_ptr<Shape>, 2> shape_queue;

  // shape size in grid size
  const std::array<int, 2> shape_size_grid{
      1,  // width
      1,  // height
  };

  std::vector<std::unique_ptr<Shape>> shape_catalogs;

 public:
  Game(const Size &grid_size, const Input &input);
  int GetDistanceOnPlaced(const Grid &grid, const Shape &shape);
  void EnrollShapeQueue();
  void FillGridContainerWithShape(const Shape &shape);
  void DrawPlaceholder(const Position placeholder_pos_grid, const Shape &shape,
                       const int cell_size_px, const Color outline_color);
  void AddScore(int adder);
  void OnUpdate();
  void Run();
};