#include <iostream>
#include <vector>
#include <raylib.h>
#include "shapes.h"

enum POS
{
  X,
  Y,
};

enum SIZE
{
  W,
  H,
};

void run(const int window_size[2]);
void draw_shape(const std::vector<std::vector<bool>> &shape, int x, int y, const int width, const int height, const u_int8_t color[4], const int grid_size[2], const int window_size[2]);

int main()
{
  int window_size[2] = {640, 800};
  const char *window_title = "Tetris";

  InitWindow(window_size[SIZE::W], window_size[SIZE::H], window_title);
  SetTargetFPS(60);

  run(window_size);

  CloseWindow();

  return 0;
}

void run(const int window_size[2])
{
  const std::vector<std::vector<std::vector<bool>>> shapes = {I, J, L, O, S, T, Z};
  // apply random color
  constexpr u_int8_t shape_color[4] = {200, 0, 0, 255};

  // grid width and height
  constexpr int grid_size[2] = {20, 40};

  // shape spawn area width from the top of screen
  constexpr int spawn_area_range[2] = {10, 20};

  // get first and second shape
  const auto shape = shapes.at(GetRandomValue(0, shapes.size() - 1)); // example

  // shape size in grid size
  const int shape_size_grid[2] = {
      1, // width
      1, // height
  };

  // shape position in grid size
  int shape_pos_grid[2] = {
      GetRandomValue(spawn_area_range[0], spawn_area_range[1]),
      // -static_cast<int>(shape.size()), // y
      0,
  };

  float move_interval_seconds = 1.f;
  float accumulated_time_seconds = 0.0f;

  // game loop
  while (!WindowShouldClose())
  {
    BeginDrawing();
    // get delta time if already one second then apply offset - 1 on y axis

    const float delta_time_seconds = GetFrameTime();
    accumulated_time_seconds += delta_time_seconds;

    ClearBackground(BLACK);

    DrawRectangle(100, 200, 100, 200, BLUE);

    if (accumulated_time_seconds >= move_interval_seconds)
    {
      accumulated_time_seconds = 0.f;
      shape_pos_grid[POS::Y] += 1;
    }

    //
    // if already on bottom then use second shape and pick new shape for the third one
    //
    // std::cout << shape_pos_grid[POS::X] << " - " << shape_pos_grid[POS::Y] << " - " << shape_size_grid[SIZE::W] << " - " << shape_size_grid[SIZE::H] << " - " << delta_time_seconds << " - " << accumulated_time_seconds << std::endl;

    draw_shape(shape, shape_pos_grid[POS::X], shape_pos_grid[POS::Y], shape_size_grid[SIZE::W], shape_size_grid[SIZE::H], shape_color, grid_size, window_size);

    EndDrawing();
  }
}

void draw_shape(const std::vector<std::vector<bool>> &shape, int x, int y, const int width, const int height, const u_int8_t color[4], const int grid_size[2], const int window_size[2])
{
  Color c{color[0], color[1], color[2], color[3]};

  for (const auto &row : shape)
  {
    int temp_x = x;

    for (const auto &column : row)
    {
      int col_x = temp_x++;

      if (!column)
      {
        continue;
      }

      // std::cout
      //     << "X"
      //     << static_cast<float>(col_x) / grid_size[SIZE::W] * window_size[SIZE::W]
      //     << "Y"
      //     << static_cast<float>(y) / grid_size[SIZE::H] * window_size[SIZE::H]
      //     << "W"
      //     << static_cast<float>(width) / grid_size[SIZE::W] * window_size[SIZE::W]
      //     << "H"
      //     << static_cast<float>(height) / grid_size[SIZE::H] * window_size[SIZE::H]
      //     << std::endl;

      DrawRectangle(
          static_cast<float>(col_x) / grid_size[SIZE::W] * window_size[SIZE::W],
          static_cast<float>(y) / grid_size[SIZE::H] * window_size[SIZE::H],
          static_cast<float>(width) / grid_size[SIZE::W] * window_size[SIZE::W],
          static_cast<float>(height) / grid_size[SIZE::H] * window_size[SIZE::H],
          c);
    }

    y++;
  }
}