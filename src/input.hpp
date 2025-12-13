#pragma once

#include "action.hpp"

class Input {
 public:
  virtual ~Input() = default;
  virtual void RegisterInputAction(ACTION &action) const = 0;
};

class KeyboardInput : public Input {
 public:
  void RegisterInputAction(ACTION &action) const override;
};