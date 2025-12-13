#include "action.hpp"
#include "raylib.h"
#include "input.hpp"

void KeyboardInput::RegisterInputAction(ACTION &action) const {
  if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) action = ACTION::MOVE_LEFT;
  if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) action = ACTION::MOVE_DOWN;
  if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT))
    action = ACTION::MOVE_RIGHT;
  if (IsKeyPressed(KEY_SPACE)) action = ACTION::FREE_FALL;
  if (IsKeyPressed(KEY_Q)) action = ACTION::ROTATE_CLOCKWISE;
  if (IsKeyPressed(KEY_E)) action = ACTION::ROTATE_COUNTERCLOCKWISE;
};