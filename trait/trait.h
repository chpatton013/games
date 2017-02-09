#pragma once

namespace trait {

template <typename Type, typename Trait>
inline constexpr const Type& cast(const Trait* trait) noexcept {
  return *static_cast<const Type*>(trait);
}

} // namespace trait
