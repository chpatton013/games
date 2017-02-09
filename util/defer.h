#pragma once

#include <functional>
#include <gtest/gtest_prod.h>
#include <memory>
#include <mutex>

namespace util {

class Defer {
public:
  using Callback = std::function<void(void)>;

  ~Defer() { this->callback(); }

  explicit Defer(Callback callback) noexcept : callback(std::move(callback)) {}
  Defer& operator=(Callback callback) noexcept {
    this->callback = std::move(callback);
    return *this;
  }

  Defer(const Defer&) = delete;
  Defer& operator=(const Defer&) = delete;

  Defer(Defer&& that) noexcept : callback(std::move(that.callback)) {}
  constexpr Defer& operator=(Defer&& that) noexcept {
    this->callback = std::move(that.callback);
    return *this;
  }

private:
  Callback callback;
};

static_assert(!std::is_default_constructible<Defer>::value,
              "Defer should not be default-constructible");
static_assert(!std::is_copy_constructible<Defer>::value,
              "Defer should not be copy-constructible");
static_assert(!std::is_copy_assignable<Defer>::value,
              "Defer should not be copy-assignable");
static_assert(std::is_move_constructible<Defer>::value,
              "Defer should be move-constructible");
static_assert(std::is_move_assignable<Defer>::value,
              "Defer should be move-assignable");

class DeferAll {
public:
  ~DeferAll() {
    if (!this->reference_count || !this->callback) {
      return;
    }

    --(*this->reference_count);
    if (*this->reference_count == 0) {
      this->callback();
    }
  }

  explicit DeferAll(Defer::Callback callback)
    : callback(std::move(callback)),
      reference_count(std::make_shared<int>(1)) {}
  DeferAll& operator=(Defer::Callback callback) noexcept {
    --(*this->reference_count);
    this->callback = std::move(callback);
    this->reference_count = std::make_shared<int>(1);
    return *this;
  }

  DeferAll(const DeferAll& that)
    : callback(that.callback), reference_count(that.reference_count) {
    ++(*this->reference_count);
  }
  constexpr DeferAll& operator=(const DeferAll& that) noexcept {
    this->callback = that.callback;
    this->reference_count = that.reference_count;
    ++(*this->reference_count);
    return *this;
  }

  DeferAll(DeferAll&&) = default;
  DeferAll& operator=(DeferAll&&) = default;

private:
  FRIEND_TEST(DeferAllTest, l_value);
  FRIEND_TEST(DeferAllTest, copied);
  FRIEND_TEST(DeferAllTest, moved);

  Defer::Callback callback;
  std::shared_ptr<int> reference_count;
};

static_assert(!std::is_default_constructible<DeferAll>::value,
              "DeferAll should not be default-constructible");
static_assert(std::is_copy_constructible<DeferAll>::value,
              "DeferAll not be copy-constructible");
static_assert(std::is_copy_assignable<DeferAll>::value,
              "DeferAll not be copy-assignable");
static_assert(std::is_move_constructible<DeferAll>::value,
              "DeferAll should be move-constructible");
static_assert(std::is_move_assignable<DeferAll>::value,
              "DeferAll should be move-assignable");

class DeferAny {
public:
  struct Synchronize {
    Synchronize() : valid(true) {}
    bool valid;
    std::mutex mutex;
  };

  ~DeferAny() { this->invoke(); }

  explicit DeferAny(Defer::Callback callback)
    : callback(std::move(callback)),
      synchronize(std::make_shared<Synchronize>()) {}
  DeferAny& operator=(Defer::Callback callback) noexcept {
    this->invoke();
    this->callback = std::move(callback);
    this->synchronize = std::make_shared<Synchronize>();
    return *this;
  }

  DeferAny(const DeferAny&) = default;
  DeferAny& operator=(const DeferAny&) = default;

  DeferAny(DeferAny&&) = default;
  DeferAny& operator=(DeferAny&&) = default;

private:
  void invoke() noexcept {
    if (!this->synchronize) {
      return;
    }

    std::lock_guard<std::mutex> guard(this->synchronize->mutex);
    if (this->synchronize->valid) {
      this->synchronize->valid = false;
      this->callback();
    }
  }

  Defer::Callback callback;
  std::shared_ptr<Synchronize> synchronize;
};

static_assert(!std::is_default_constructible<DeferAny>::value,
              "DeferAny should not be default-constructible");
static_assert(std::is_copy_constructible<DeferAny>::value,
              "DeferAny should be copy-constructible");
static_assert(std::is_copy_assignable<DeferAny>::value,
              "DeferAny should be copy-assignable");
static_assert(std::is_move_constructible<DeferAny>::value,
              "DeferAny should be move-constructible");
static_assert(std::is_move_assignable<DeferAny>::value,
              "DeferAny should be move-assignable");

} // namespace util
