#pragma once

#include "trait/eq_ops.h"
#include "trait/to_string.h"
#include <cstdint>
#include <glog/logging.h>
#include <mutex>
#include <ostream>
#include <type_traits>
#include <utility>

namespace util {

using Id = std::uint64_t;

struct IdRange;

inline constexpr bool operator==(const IdRange&, const IdRange&) noexcept;
inline std::ostream& operator<<(std::ostream&, const IdRange&) noexcept;

struct IdRange : public trait::EqOps<IdRange>, public trait::ToString<IdRange> {
  constexpr IdRange(Id begin, Id end) noexcept
    : begin(std::move(begin)), end(std::move(end)) {
    CHECK_LE(begin, end) << "Invalid IdRange";
  }

  const Id begin;
  const Id end;

  constexpr std::size_t size() const noexcept {
    return this->end - this->begin;
  }

  constexpr bool empty() const noexcept { return this->size() == 0; }
};

constexpr bool operator==(const IdRange& lhs, const IdRange& rhs) noexcept {
  return lhs.begin == rhs.begin && lhs.end == rhs.end;
}

std::ostream& operator<<(std::ostream& out, const IdRange& range) noexcept {
  return out << "IdRange[" << range.begin << "," << range.end << ")";
}

class Identifier {
public:
  static constexpr Id InvalidId() noexcept { return 0; }

  Identifier() noexcept : Identifier(1) {}
  explicit Identifier(Id id) noexcept : id(std::move(id)), mutex() {}

  Identifier(const Identifier&) = delete;
  Identifier& operator=(const Identifier&) = delete;

  Identifier(Identifier&& that) noexcept {
    std::lock_guard<std::mutex> lock(that.mutex);

    this->id = std::move(that.id);
    that.id = InvalidId();
  }
  Identifier& operator=(Identifier&& that) noexcept {
    CHECK(this != &that) << "Cannot move-assign from the same instance";

    std::lock_guard<std::mutex> this_lock(this->mutex);
    std::lock_guard<std::mutex> that_lock(that.mutex);

    this->id = std::move(that.id);
    that.id = InvalidId();
    return *this;
  }

  bool valid() const noexcept { return this->id != InvalidId(); }

  void check() const noexcept {
    CHECK(this->valid()) << "Identifier is invalid";
  }

  Id last() const noexcept {
    this->check();
    return this->id - 1;
  }

  const Id& next() const noexcept {
    this->check();
    return this->id;
  }

  Id reserve() noexcept { return this->reserve(1).begin; }

  IdRange reserve(std::size_t increment) noexcept {
    CHECK(increment > 0) << "Cannot reserve 0 elements";
    std::lock_guard<std::mutex> lock(this->mutex);
    this->check();

    const auto begin = this->id;
    this->id += increment;
    return IdRange(begin, this->id);
  }

private:
  Id id;
  mutable std::mutex mutex;
};

static_assert(std::is_default_constructible<Identifier>::value,
              "Identifier should be default-constructible");
static_assert(!std::is_copy_constructible<Identifier>::value,
              "Identifier should not be copy-constructible");
static_assert(!std::is_copy_assignable<Identifier>::value,
              "Identifier should not be copy-assignable");
static_assert(std::is_move_constructible<Identifier>::value,
              "Identifier should be move-constructible");
static_assert(std::is_move_assignable<Identifier>::value,
              "Identifier should be move-assignable");

} // namespace util
