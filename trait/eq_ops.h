#pragma once

#include "trait/trait.h"

namespace trait {

/**
 * Extend this trait to inherit simple equality operators based on your object's
 * const operator== overload. Note that your object must have this overload
 * implemented to use this trait.
 */
template <typename Lhs, typename Rhs = Lhs>
class EqOps {
public:
  constexpr bool operator==(const Rhs& that) noexcept {
    return cast<Lhs>(this) == that;
  }
  constexpr bool operator!=(const Rhs& that) noexcept {
    return !(cast<Lhs>(this) == that);
  }
  constexpr bool operator!=(const Rhs& that) const noexcept {
    return !(cast<Lhs>(this) == that);
  }

protected:
  ~EqOps() = default;
};

} // namespace trait
