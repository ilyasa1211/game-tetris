// 2d representation (height x width)
#include "shapes.hpp"

#include <vector>

namespace shape {
const std::vector<std::vector<bool>> I = {
    {1},
    {1},
    {1},
    {1},
};
const std::vector<std::vector<bool>> J = {
    {0, 1},
    {0, 1},
    {0, 1},
    {1, 1},
};
const std::vector<std::vector<bool>> L = {
    {1, 0},
    {1, 0},
    {1, 0},
    {1, 1},
};
const std::vector<std::vector<bool>> O = {
    {1, 1},
    {1, 1},
};
const std::vector<std::vector<bool>> S = {
    {0, 1, 1},
    {1, 1, 0},
};
const std::vector<std::vector<bool>> T = {
    {1, 1, 1},
    {0, 1, 0},
};
const std::vector<std::vector<bool>> Z = {
    {1, 1, 0},
    {0, 1, 1},
};

}  // namespace shapes