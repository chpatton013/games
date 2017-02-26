#pragma once

#include "trait/rel_ops.h"
#include "trait/to_string.h"
#include <algorithm>
#include <cstdint>
#include <glog/logging.h>
#include <ostream>
#include <type_traits>

namespace util {

struct StaticString;

inline std::ostream& operator<<(std::ostream& out,
                                const StaticString& str) noexcept;

inline constexpr bool operator==(const StaticString& lhs,
                                 const StaticString& rhs) noexcept;

inline constexpr bool operator<(const StaticString& lhs,
                                const StaticString& rhs) noexcept;

inline constexpr bool operator==(const StaticString& lhs,
                                 const char* rhs) noexcept;

inline constexpr bool operator<(const StaticString& lhs,
                                const char* rhs) noexcept;

struct StaticString : public trait::ToString<StaticString>,
                      public trait::RelOps<StaticString>,
                      public trait::RelOps<StaticString, const char*> {
  template <std::size_t Size>
  constexpr StaticString(const char (&data)[Size]) noexcept
    : data(data), size(Size - 1) {
    CHECK(Size > 0) << "Size is invalid";
    CHECK(this->data[this->size] == '\0') << "String is not null-terminated";
  }

  constexpr char at(std::size_t index) const noexcept {
    CHECK(index < this->size) << "Index out of range: " << index
                              << " not in [0," << (this->size - 1) << "]";
    return this->data[index];
  }

  constexpr char operator[](std::size_t index) const noexcept {
    return this->at(index);
  }

  constexpr bool empty() const noexcept { return this->size == 0; }

  constexpr operator const char*() const noexcept { return this->data; }

  const char* data;
  const std::size_t size;
};

static_assert(!std::is_default_constructible<StaticString>::value,
              "StaticString should not be default-constructible");
static_assert(std::is_copy_constructible<StaticString>::value,
              "StaticString should be copy-constructible");
static_assert(!std::is_copy_assignable<StaticString>::value,
              "StaticString should not be copy-assignable");
static_assert(std::is_move_constructible<StaticString>::value,
              "StaticString should be move-constructible");
static_assert(!std::is_move_assignable<StaticString>::value,
              "StaticString should not be move-assignable");
static_assert(std::is_literal_type<StaticString>::value,
              "StaticString should be a literal type");

static_assert(StaticString("").empty(), "Empty StaticString should be empty");
static_assert(!StaticString("abc").empty(),
              "Non-empty StaticString should not be empty");

namespace internal {

enum class StaticStringComparison {
  Equal,
  Less,
  More,
};

constexpr static std::size_t StaticStringLength(const char* data) noexcept {
  if (*data == '\0') {
    return 0;
  } else {
    return StaticStringLength(data + 1) + 1;
  }
}

constexpr static StaticStringComparison
StaticStringCompare(const char* lhs_data,
                    std::size_t lhs_size,
                    const char* rhs_data,
                    std::size_t rhs_size) {
  for (std::size_t index = 0; index < std::min(lhs_size, rhs_size); ++index) {
    if (lhs_data[index] < rhs_data[index]) {
      return StaticStringComparison::Less;
    } else if (lhs_data[index] > rhs_data[index]) {
      return StaticStringComparison::More;
    }
  }

  if (lhs_size < rhs_size) {
    return StaticStringComparison::Less;
  } else if (lhs_size > rhs_size) {
    return StaticStringComparison::More;
  } else {
    return StaticStringComparison::Equal;
  }
}

constexpr static StaticStringComparison
StaticStringCompare(const StaticString& lhs, const char* rhs) {
  return StaticStringCompare(lhs.data, lhs.size, rhs, StaticStringLength(rhs));
}

constexpr static StaticStringComparison
StaticStringCompare(const StaticString& lhs, const StaticString& rhs) {
  return StaticStringCompare(lhs.data, lhs.size, rhs.data, rhs.size);
}

} // namespace internal

std::ostream& operator<<(std::ostream& out, const StaticString& str) noexcept {
  return out << static_cast<const char*>(str);
}

constexpr bool operator==(const StaticString& lhs,
                          const StaticString& rhs) noexcept {
  return internal::StaticStringCompare(lhs, rhs) ==
      internal::StaticStringComparison::Equal;
}

constexpr bool operator<(const StaticString& lhs,
                         const StaticString& rhs) noexcept {
  return internal::StaticStringCompare(lhs, rhs) ==
      internal::StaticStringComparison::Less;
}

constexpr bool operator==(const StaticString& lhs, const char* rhs) noexcept {
  return internal::StaticStringCompare(lhs, rhs) ==
      internal::StaticStringComparison::Equal;
}

constexpr bool operator<(const StaticString& lhs, const char* rhs) noexcept {
  return internal::StaticStringCompare(lhs, rhs) ==
      internal::StaticStringComparison::Less;
}

static_assert(StaticString("") == StaticString(""), "");
static_assert(StaticString("abc") == StaticString("abc"), "");
static_assert(StaticString("abc") != StaticString(""), "");
static_assert(StaticString("abc") != StaticString("cba"), "");
static_assert(StaticString("") < StaticString("abc"), "");
static_assert(StaticString("ab") < StaticString("abc"), "");
static_assert(StaticString("abc") < StaticString("cba"), "");

} // namespace util
