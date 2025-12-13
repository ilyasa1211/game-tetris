#include "game.hpp"

#include <raylib.h>

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "action.hpp"
#include "grid.hpp"
#include "input.hpp"
#include "position.hpp"
#include "shapes.hpp"
#include "size.hpp"

Game::Game(const Size &grid_size, const Input &input)
    : input{input}, grid(Grid(grid_size.w, grid_size.h)) {
  this->shape_catalogs.push_back(
      std::make_unique<IShape>(0, 0, this->shape_color_rgba));
  this->shape_catalogs.push_back(
      std::make_unique<JShape>(0, 0, this->shape_color_rgba));
  this->shape_catalogs.push_back(
      std::make_unique<LShape>(0, 0, this->shape_color_rgba));
  this->shape_catalogs.push_back(
      std::make_unique<OShape>(0, 0, this->shape_color_rgba));
  this->shape_catalogs.push_back(
      std::make_unique<SShape>(0, 0, this->shape_color_rgba));
  this->shape_catalogs.push_back(
      std::make_unique<TShape>(0, 0, this->shape_color_rgba));
  this->shape_catalogs.push_back(
      std::make_unique<ZShape>(0, 0, this->shape_color_rgba));

  shape_queue[0] =
      shape_catalogs.at(GetRandomValue(0, shape_catalogs.size() - 1))->Clone();
  shape_queue[1] =
      shape_catalogs.at(GetRandomValue(0, shape_catalogs.size() - 1))->Clone();

  shape_queue.at(0)->SetPositionX(
      GetRandomValue(spawn_area_range[0], spawn_area_range[1]));
  shape_queue.at(0)->SetPositionY(0);
};

int Game::GetDistanceOnPlaced(const Grid &grid, const Shape &shape) {
  const int x = shape.GetPosition().x;
  const int y = shape.GetPosition().y;

  int least_distance = grid.GetHeight() - 1;

  for (int col = 0; col < shape.GetWidth(); col++) {
    int offset = 0;

    for (int row = shape.GetHeight() - 1; row >= 0; row--) {
      if (!shape.Get().at(row).at(col)) {
        continue;
      }

      for (int n = y + row; n < grid.GetHeight() - 1; n++) {
        if (grid.Get().at(n + 1).at(x + col)) {
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
};

void Game::EnrollShapeQueue() {
  this->shape_queue[0] = std::move(this->shape_queue[1]);
  this->shape_queue[1] =
      shape_catalogs.at(GetRandomValue(0, shape_catalogs.size() - 1))->Clone();
}

void Game::FillGridContainerWithShape(const Shape &shape) {
  int y = shape.GetPosition().y;

  for (const auto &row : shape.Get()) {
    int temp_x = shape.GetPosition().x;

    for (auto column : row) {
      int col_x = temp_x++;

      if (!column) {
        continue;
      }
      this->grid.Set(col_x, y - 1, column);
    }

    y++;
  }
}

void Game::DrawPlaceholder(const Position placeholder_pos_grid,
                           const Shape &shape, const int cell_size_px,
                           const Color outline_color) {
  for (int i = 0; i < shape.Get().size(); i++) {
    for (int j = 0; j < shape.Get().at(0).size(); j++) {
      if (!shape.Get()[i][j]) {
        continue;
      }
      DrawRectangleLines((placeholder_pos_grid.x + j) * cell_size_px,
                         (placeholder_pos_grid.y + i) * cell_size_px,
                         cell_size_px, cell_size_px, outline_color);
    }
  }
};

void Game::AddScore(int adder) { this->scores += adder; };

void Game::OnUpdate() {
  this->input.RegisterInputAction(this->action);

  auto& shape = shape_queue[0];

  // get delta time if already passed interval limit
  // then apply offset + 1 on y axis
  const float delta_time_seconds = GetFrameTime();
  accumulated_time_seconds += delta_time_seconds;
  if (accumulated_time_seconds >= move_interval_seconds) {
    accumulated_time_seconds = 0.f;
    shape->SetPositionY(shape->GetPosition().y + 1);
  }

  accumulated_auto_score_seconds += delta_time_seconds;
  if (accumulated_auto_score_seconds >= auto_score_interval_seconds) {
    accumulated_auto_score_seconds = 0.f;
    this->AddScore(this->score_multiplier);
  }
  time_played_seconds += delta_time_seconds;

  Position placeholder_pos_grid = {
      .x = shape->GetPosition().x,
      .y = shape->GetPosition().y +
           this->GetDistanceOnPlaced(this->grid, *shape),
  };

  switch (action) {
    case ACTION::MOVE_LEFT: {
      bool is_full_to_the_left = shape->GetPosition().x <= 0;
      if (is_full_to_the_left ||
          shape->IsTouchedOnTheNextXAxis(this->grid, -1)) {
        break;
      }

      shape->SetPositionX(shape->GetPosition().x - 1);
      break;
    }
    case ACTION::MOVE_RIGHT: {
      bool is_full_to_the_right =
          shape->GetPosition().x + shape->GetWidth() >= this->grid.GetWidth();
      if (is_full_to_the_right ||
          shape->IsTouchedOnTheNextXAxis(this->grid, +1)) {
        break;
      }
      shape->SetPositionX(shape->GetPosition().x + 1);
      break;
    }
    case ACTION::MOVE_DOWN:
      if (shape->GetPosition().y + shape->GetHeight() - 1 >=
          this->grid.GetHeight()) {
        break;
      }
      shape->SetPositionY(shape->GetPosition().y + 1);
      break;
    case ACTION::FREE_FALL: {
      shape->SetPositionY(placeholder_pos_grid.y);

      break;
    }
    case ACTION::ROTATE_CLOCKWISE: {
      shape = shape->RotateShapeClockwise();
      break;
    }
    case ACTION::ROTATE_COUNTERCLOCKWISE: {
      shape = shape->RotateShapeCounterclockwise();
      break;
    }
    default:
      break;
  }

  this->action = ACTION::NONE;

  if (shape->IsOnGround(this->grid)) {
    this->FillGridContainerWithShape(*shape);

    // use the next shape
    this->EnrollShapeQueue();

    shape->SetPositionY(0);
    shape->SetPositionX(
        GetRandomValue(spawn_area_range[0], spawn_area_range[1]));
  }

  const int rows_affected = this->grid.EraseCompletedRows();

  if (rows_affected > 0) {
    this->scores += rows_affected * this->score_multiplier;
    this->grid.ShiftDownFloatingRows();
  }

  // render
  BeginDrawing();
  ClearBackground(BLACK);

  shape->Draw(this->cell_size_px);
  this->grid.Draw(this->cell_size_px, this->cell_color_rgba);

  this->DrawPlaceholder(placeholder_pos_grid, *shape, this->cell_size_px,
                        this->placeholder_color_rgba);

  std::string score_text = "Scores: " + std::to_string(scores);
  std::string time_played_text =
      "Time played: " + std::to_string(static_cast<int>(time_played_seconds)) +
      " seconds";

  DrawText(score_text.c_str(), 0, 0, 40, WHITE);
  DrawText(time_played_text.c_str(), 0, 40, 20, WHITE);

  EndDrawing();
};

void Game::Run() {
  // game loop
#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(this->OnUpdate, 0, 1);
#else
  SetTargetFPS(this->target_fps);
  // Main game loop
  while (!WindowShouldClose())  // Detect window close
                                // button or ESC key
  {
    this->OnUpdate();
  }
#endif
};