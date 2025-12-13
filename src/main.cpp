#include <raylib.h>

#include <array>
#include <iostream>
#include <memory>
#include <string>

#include "game.hpp"
#include "input.hpp"
#include "size.hpp"

int main() {
  constexpr Size window_size = {
    .w = 400, 
    .h = 800,
  };
  constexpr Size grid_size = {
    .w = 20, 
    .h = 40,
  };
  std::string window_title = "Tetris";

  InitWindow(window_size.w, window_size.h, window_title.c_str());

  std::cout << "Starting Tetris." << "\n";

  const std::unique_ptr<Input> input = std::make_unique<KeyboardInput>();
  Game game(grid_size, *input);

  game.Run();

  CloseWindow();

  return 0;
}
