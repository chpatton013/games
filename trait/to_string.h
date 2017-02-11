#pragma once

#include "trait/trait.h"
#include <sstream>
#include <string>

namespace trait {

/**
 * Extend this trait to inherit a simple to_string function based on your
 * object's const operator<<(std::ostream&) overload. Note that your object must
 * have this overload implemented to use this trait.
 */
template <typename T>
class ToString {
public:
  std::string to_string() const noexcept {
    std::stringstream ss;
    ss << cast<T>(this);
    return ss.str();
  }

  operator std::string() const noexcept { return this->to_string(); }

protected:
  ~ToString() = default;
};

} // namespace trait
