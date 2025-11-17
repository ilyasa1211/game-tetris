#include <raylib.h>

#include <array>
#include <iostream>
#include <vector>

#include "shapes.h"

enum POS {
  X,
  Y,
};

enum SIZE {
  W,
  H,
};

enum ACTION {
  NONE,
  MOVE_RIGHT,
  MOVE_LEFT,
  MOVE_DOWN,
  FREE_FALL,
};

void run(const std::array<int, 2> window_size);
void draw_shape(const std::vector<std::vector<bool>> &shape, int x, int y,
                const int width, const int height,
                const std::array<u_int8_t, 4> color,
                const std::array<int, 2> grid_size,
                const std::array<int, 2> window_size, const int cell_size_px);
template <size_t W, size_t H>
void draw_grid(const std::array<std::array<bool, W>, H> &grid_container,
               const std::array<int, 2> grid_size,
               const std::array<int, 2> window_size, const int cell_size,
               const std::array<u_int8_t, 4> cell_color);

template <size_t W, size_t H>
void fill_grid_container(std::array<std::array<bool, W>, H> &grid_container,
                         const std::vector<std::vector<bool>> &shape,
                         const std::array<int, 2> shape_pos_grid);

void enroll_shape_queue(
    std::array<std::vector<std::vector<bool>>, 2> &shape_queue,
    const std::vector<std::vector<std::vector<bool>>> &shapes);

template <size_t W, size_t H>
bool is_on_top_of_another_shape(
    const std::array<std::array<bool, W>, H> &grid_container,
    const std::vector<std::vector<bool>> &shape,
    const std::array<int, 2> shape_pos_grid);

template <size_t W, size_t H>
bool is_touched_on_the_next_x_axis(
    const std::array<std::array<bool, W>, H> &grid_container,
    const std::vector<std::vector<bool>> &shape,
    const std::array<int, 2> shape_pos_grid,
    const int offset_x  // -1 (left) or 1 (right)
);
template <size_t W, size_t H>
int erase_completed_rows(std::array<std::array<bool, W>, H> &grid_container);
template <size_t W, size_t H>
void erase_row(std::array<std::array<bool, W>, H> &grid_container,
               const int row);

template <size_t W, size_t H>
void shift_down_floating_rows(
    std::array<std::array<bool, W>, H> &grid_container);

template <size_t W, size_t H>
int get_distance_on_placed(std::array<std::array<bool, W>, H> &grid_container,
                           const std::vector<std::vector<bool>> &shape,
                           const std::array<int, 2> &shape_pos_grid);
int main() {
  std::array<int, 2> window_size = {640, 800};
  std::string window_title = "Tetris";

  InitWindow(window_size[SIZE::W], window_size[SIZE::H], window_title.c_str());
  SetTargetFPS(60);

  run(window_size);

  CloseWindow();

  return 0;
}

void run(const std::array<int, 2> window_size) {
  // grid width and height
  constexpr std::array<int, 2> grid_size = {20, 40};

  std::array<std::array<bool, grid_size[SIZE::W]>, grid_size[SIZE::H]>
      grid_container = {};
  const int cell_size_px = 20;
  const std::array<u_int8_t, 4> cell_color_rgba = {100, 200, 150, 255};

  // shape spawn area width from the top of screen
  constexpr std::array<int, 2> spawn_area_range = {5, 15};

  const std::vector<std::vector<std::vector<bool>>> shapes = {I, J, L, O,
                                                              S, T, Z};

  // apply random color
  constexpr std::array<u_int8_t, 4> shape_color_rgba = {200, 100, 0,
                                                        255};  // red

  // get first and second shape
  std::array<std::vector<std::vector<bool>>, 2> shape_queue = {
      shapes.at(GetRandomValue(0, shapes.size() - 1)),
      shapes.at(GetRandomValue(0, shapes.size() - 1)),
  };

  // shape size in grid size
  const std::array<int, 2> shape_size_grid = {
      1,  // width
      1,  // height
  };

  // shape position in grid size
  std::array<int, 2> shape_pos_grid = {
      GetRandomValue(spawn_area_range[0], spawn_area_range[1]),
      // -static_cast<int>(shape.size()), // y
      0,
  };

  float move_interval_seconds = 0.5f;
  float accumulated_time_seconds = 0.0f;

  ACTION action = ACTION::NONE;

  // game loop
  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))
      action = ACTION::MOVE_LEFT;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) action = ACTION::MOVE_DOWN;
    if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT))
      action = ACTION::MOVE_RIGHT;
    if (IsKeyPressed(KEY_SPACE)) action = ACTION::FREE_FALL;

    // get delta time if already passed interval limit then apply offset + 1 on
    // y axis
    const float delta_time_seconds = GetFrameTime();
    accumulated_time_seconds += delta_time_seconds;
    if (accumulated_time_seconds >= move_interval_seconds) {
      accumulated_time_seconds = 0.f;
      shape_pos_grid[POS::Y] += 1;
    }

    auto shape = shape_queue[0];

    switch (action) {
      case ACTION::MOVE_LEFT: {
        bool is_full_to_the_left = shape_pos_grid[POS::X] <= 0;
        if (is_full_to_the_left ||
            is_touched_on_the_next_x_axis(grid_container, shape, shape_pos_grid,
                                          -1)) {
          break;
        }
        shape_pos_grid[POS::X] -= 1;
        break;
      }
      case ACTION::MOVE_RIGHT: {
        bool is_full_to_the_right =
            shape_pos_grid[POS::X] + shape.at(0).size() >= grid_size[SIZE::W];
        if (is_full_to_the_right ||
            is_touched_on_the_next_x_axis(grid_container, shape, shape_pos_grid,
                                          1)) {
          break;
        }
        shape_pos_grid[POS::X] += 1;
        break;
      }
      case ACTION::MOVE_DOWN:
        if (shape_pos_grid[POS::Y] + shape.size() - 1 >= grid_size[SIZE::H]) {
          break;
        }
        shape_pos_grid[POS::Y] += 1;
        break;
      case ACTION::FREE_FALL: {
        shape_pos_grid[POS::Y] += get_distance_on_placed(grid_container, shape, shape_pos_grid);
        break;
      }
      default:
        break;
    }

    action = ACTION::NONE;

    const bool on_bottom =
        shape_pos_grid[POS::Y] + shape.size() - 1 >= grid_size[SIZE::H];

    if (on_bottom ||
        is_on_top_of_another_shape(grid_container, shape, shape_pos_grid)) {
      fill_grid_container(grid_container, shape, shape_pos_grid);

      shape_pos_grid[POS::Y] = 0;
      shape_pos_grid[POS::X] =
          GetRandomValue(spawn_area_range[0], spawn_area_range[1]),

      // use the next shape
          enroll_shape_queue(shape_queue, shapes);
    }

    if (erase_completed_rows(grid_container) > 0) {
      shift_down_floating_rows(grid_container);
    }

    // render
    BeginDrawing();
    ClearBackground(BLACK);

    draw_shape(shape, shape_pos_grid[POS::X], shape_pos_grid[POS::Y],
               shape_size_grid[SIZE::W], shape_size_grid[SIZE::H],
               shape_color_rgba, grid_size, window_size, cell_size_px);

    draw_grid(grid_container, grid_size, window_size, cell_size_px,
              cell_color_rgba);

    EndDrawing();
  }
}

// returns how many rows that are erased
template <size_t W, size_t H>
int erase_completed_rows(std::array<std::array<bool, W>, H> &grid_container) {
  int erased_rows = 0;

  for (int i = grid_container.size() - 1; i >= 0; i--) {
    int block_count = 0;
    for (auto &&col : grid_container.at(i)) {
      if (!col) {
        break;
      }
      block_count++;
    }

    if (block_count == grid_container.at(i).size()) {
      erase_row(grid_container, i);
      erased_rows++;
    }
  }

  return erased_rows;
}

template <size_t W, size_t H>
void erase_row(std::array<std::array<bool, W>, H> &grid_container,
               const int row) {
  for (auto &&i : grid_container.at(row)) {
    i = 0;
  }
}

// measures how much distance on y-axis until it reach the bottom or on top of
// another shape.
template <size_t W, size_t H>
int get_distance_on_placed(std::array<std::array<bool, W>, H> &grid_container,
                           const std::vector<std::vector<bool>> &shape,
                           const std::array<int, 2> &shape_pos_grid) {
  const int x = shape_pos_grid[POS::X];
  const int y = shape_pos_grid[POS::Y];

  int least_distance = grid_container.size() - 1;

  for (size_t i = 0; i < shape.at(0).size(); i++) {
    int offset = 0;

    for (int j = shape.size() - 1; j >= 0; j--) {
      if (!shape.at(j).at(i)) {
        offset++;
        continue;
      }

      for (size_t n = y + j; n < grid_container.size() - 1; n++) {
        if (grid_container.at(n + 1).at(x + i)) {
          break;
        }

        offset++;
      }

      break;
    }

    if (offset < least_distance) {
      least_distance = offset;
    }
  }

  return least_distance;
}

template <size_t W, size_t H>
void shift_down_floating_rows(
    std::array<std::array<bool, W>, H> &grid_container) {
  int grid_column_size = grid_container.at(0).size();

  for (size_t col = 0; col < grid_column_size; col++) {
    int offset = 0;

    for (size_t row = grid_container.size() - 1; row >= 1; row--) {
      bool *val = &grid_container.at(row).at(col);

      if (!*val) {
        offset++;
        continue;
      }

      grid_container.at(row + offset).at(col) = *val;
      *val = false;
    }
  }
}

void enroll_shape_queue(
    std::array<std::vector<std::vector<bool>>, 2> &shape_queue,
    const std::vector<std::vector<std::vector<bool>>> &shapes) {
  shape_queue[0] = shape_queue[1];
  shape_queue[1] = shapes.at(GetRandomValue(0, shapes.size() - 1));
}

template <size_t W, size_t H>
bool is_touched_on_the_next_x_axis(
    const std::array<std::array<bool, W>, H> &grid_container,
    const std::vector<std::vector<bool>> &shape,
    const std::array<int, 2> shape_pos_grid,
    const int offset_x  // -1 (left) or 1 (right)
) {
  int y = shape_pos_grid[POS::Y];

  for (const auto &row : shape) {
    int x = shape_pos_grid[POS::X];

    for (const auto &col : row) {
      int col_x = x++;
      if (!col) {
        continue;
      }

      if (grid_container.at(y).at(col_x + offset_x)) {
        return true;
      }
    }

    y++;
  }
  return false;
}

template <size_t W, size_t H>
bool is_on_top_of_another_shape(
    const std::array<std::array<bool, W>, H> &grid_container,
    const std::vector<std::vector<bool>> &shape,
    const std::array<int, 2> shape_pos_grid) {
  int y = shape_pos_grid[POS::Y];

  for (const auto &row : shape) {
    int x = shape_pos_grid[POS::X];

    for (const auto &col : row) {
      int col_x = x++;
      if (!col) {
        continue;
      }

      if (grid_container.at(y).at(col_x)) {
        return true;
      }
    }

    y++;
  }
  return false;
}

template <size_t W, size_t H>
void fill_grid_container(std::array<std::array<bool, W>, H> &grid_container,
                         const std::vector<std::vector<bool>> &shape,
                         const std::array<int, 2> shape_pos_grid) {
  int y = shape_pos_grid[POS::Y];

  for (const auto &row : shape) {
    int temp_x = shape_pos_grid[POS::X];

    for (auto column : row) {
      int col_x = temp_x++;

      if (!column) {
        continue;
      }
      grid_container.at(y - 1).at(col_x) = column;
    }

    y++;
  }
}

template <size_t W, size_t H>
void draw_grid(const std::array<std::array<bool, W>, H> &grid_container,
               const std::array<int, 2> grid_size,
               const std::array<int, 2> window_size, const int cell_size_px,
               const std::array<u_int8_t, 4> cell_color) {
  Color c = {cell_color[0], cell_color[1], cell_color[2], cell_color[3]};

  for (size_t i = 0; i < grid_container.size(); i++) {
    for (size_t j = 0; j < grid_container.at(i).size(); j++) {
      if (grid_container[i][j]) {
        // std::cout << j << " - " << i << std::endl;

        DrawRectangle(
            static_cast<float>(j) / grid_size[SIZE::W] * window_size[SIZE::W],
            static_cast<float>(i) / grid_size[SIZE::H] * window_size[SIZE::H],
            cell_size_px, cell_size_px, c);
      }
    }
  }
}

void draw_shape(const std::vector<std::vector<bool>> &shape, int x, int y,
                const int width, const int height,
                const std::array<u_int8_t, 4> color,
                const std::array<int, 2> grid_size,
                const std::array<int, 2> window_size, const int cell_size_px) {
  Color c{color[0], color[1], color[2], color[3]};

  for (const auto &row : shape) {
    int temp_x = x;

    for (const auto &column : row) {
      int col_x = temp_x++;

      if (!column) {
        continue;
      }

      DrawRectangle(
          static_cast<float>(col_x) / grid_size[SIZE::W] * window_size[SIZE::W],
          static_cast<float>(y) / grid_size[SIZE::H] * window_size[SIZE::H],
          cell_size_px, cell_size_px, c);
    }

    y++;
  }
}