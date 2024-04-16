#pragma once

#include <cstdint>
#include <cmath>

#include <array>
#include <string>
#include <concepts>
#include <type_traits>

namespace cpa {

//used with std::conditional_t to delete specific struct fields
using zero_t = int[0];
static_assert(sizeof(zero_t) == 0, "zero_t is not zero!");

struct _range {
  struct iterator {
    iterator(int64_t position, int64_t step = 0) : position(position), step(step) {}
    auto operator*() const -> int64_t { return position; }
    auto operator!=(const iterator& source) const -> bool { return step > 0 ? position < source.position : position > source.position; }
    auto operator++() -> iterator& { position += step; return *this; }
    
  private:
    int64_t position;
    const int64_t step;
  };
  
  auto begin() const -> iterator { return {origin, stride}; }
  auto end() const -> iterator { return {target}; }
  
  int64_t origin;
  int64_t target;
  int64_t stride;
};

static inline auto range(int64_t sz) {
  return _range { 0, sz, 1 };
}

static constexpr float EPSILON = 1e-6;

}
