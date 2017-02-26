#pragma once

#include <boost/optional.hpp>
#include <cstdint>
#include <glog/logging.h>
#include <mutex>
#include <type_traits>
#include <utility>

namespace util {

using Id = uint64_t;

class Identifier {
public:
  static constexpr const Id kInvalidId = 0;

  Identifier() : Identifier(1) {}
  explicit Identifier(Id id) : id(std::move(id)), mutex() {}

  Identifier(const Identifier&) = delete;
  Identifier& operator=(const Identifier&) = delete;

  Identifier(Identifier&& that) : Identifier(std::move(that.id)) {
    that.id = kInvalidId;
  }
  Identifier& operator=(Identifier&& that) {
    this->id = std::move(that.id);
    that.id = kInvalidId;
    return *this;
  }

  Id reserve() {
    std::lock_guard<std::mutex> lock(mutex);
    return id++;
  }

  const Id& next() const {
    std::lock_guard<std::mutex> lock(mutex);
    return id;
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
