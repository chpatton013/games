#pragma once

#include "trait/trait.h"

namespace trait {

/**
 * Extend this trait to inherit simple relational operators based on your
 * object's const operator== and operator< overloads. Note that your object must
 * have these overloads implemented to use this trait.
 */
template <typename Lhs, typename Rhs = Lhs>
class RelOps {
public:
  constexpr bool operator==(const Rhs& that) noexcept {
    return cast<Lhs>(this) == that;
  }
  constexpr bool operator<(const Rhs& that) noexcept {
    return cast<Lhs>(this) < that;
  }
  constexpr bool operator!=(const Rhs& that) noexcept {
    return !(cast<Lhs>(this) == that);
  }
  constexpr bool operator>(const Rhs& that) noexcept {
    return that < cast<Lhs>(this);
  }
  constexpr bool operator<=(const Rhs& that) noexcept {
    return !(that < cast<Lhs>(this));
  }
  constexpr bool operator>=(const Rhs& that) noexcept {
    return !(cast<Lhs>(this) < that);
  }

  constexpr bool operator!=(const Rhs& that) const noexcept {
    return !(cast<Lhs>(this) == that);
  }
  constexpr bool operator>(const Rhs& that) const noexcept {
    return that < cast<Lhs>(this);
  }
  constexpr bool operator<=(const Rhs& that) const noexcept {
    return !(that < cast<Lhs>(this));
  }
  constexpr bool operator>=(const Rhs& that) const noexcept {
    return !(cast<Lhs>(this) < that);
  }
};

} // namespace trait
