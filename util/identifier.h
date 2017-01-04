#pragma once

#include <mutex>

namespace util {

using Id = uint64_t;

class Identifier {
public:
  Identifier() : Identifier(0) {}
  explicit Identifier(const Id& id) : id_(id), mutex_() {}

  ~Identifier() = default;
  Identifier(const Identifier&) = delete;
  Identifier(Identifier&&) = default;
  Identifier& operator=(const Identifier&) = delete;
  Identifier& operator=(Identifier&&) = default;

  Id reserve() {
    std::lock_guard<std::mutex> lock(mutex_);
    return id_++;
  }

  const Id& next() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return id_;
  }

private:
  Id id_;
  mutable std::mutex mutex_;
};

} // namespace util
