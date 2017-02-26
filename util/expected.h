#pragma once

#include "trait/eq_ops.h"
#include "trait/to_string.h"
#include "util/macros.h"
#include <boost/variant.hpp>
#include <glog/logging.h>
#include <ostream>
#include <type_traits>
#include <utility>

namespace util {

template <typename ValueType, typename ErrorType>
class Expected;

template <typename ValueType, typename ErrorType>
inline constexpr std::ostream&
operator<<(std::ostream& out,
           const Expected<ValueType, ErrorType>& expected) noexcept;

template <typename ValueType, typename ErrorType>
inline constexpr bool
operator==(const Expected<ValueType, ErrorType>& lhs,
           const Expected<ValueType, ErrorType>& rhs) noexcept;

template <typename ValueType, typename ErrorType>
inline constexpr bool operator==(const Expected<ValueType, ErrorType>& lhs,
                                 const ValueType& rhs) noexcept;

template <typename ValueType, typename ErrorType>
inline constexpr bool operator==(const Expected<ValueType, ErrorType>& lhs,
                                 const ErrorType& rhs) noexcept;

template <typename ValueType, typename ErrorType>
class Expected
    : public trait::ToString<Expected<ValueType, ErrorType>>,
      public trait::EqOps<Expected<ValueType, ErrorType>>,
      public trait::EqOps<Expected<ValueType, ErrorType>, ValueType>,
      public trait::EqOps<Expected<ValueType, ErrorType>, ErrorType> {
public:
  Expected() = delete;

  constexpr Expected(const ValueType& value) noexcept : variant(value){};
  constexpr Expected& operator=(const ValueType& value) noexcept {
    this->variant = value;
    return *this;
  }

  constexpr Expected(ValueType&& value) noexcept : variant(std::move(value)) {}
  constexpr Expected& operator=(ValueType&& value) noexcept {
    this->variant = std::move(value);
    return *this;
  }

  constexpr Expected(const ErrorType& error) noexcept : variant(error) {}
  constexpr Expected& operator=(const ErrorType& error) noexcept {
    this->variant = error;
    return *this;
  }

  constexpr Expected(ErrorType&& error) noexcept : variant(std::move(error)) {}
  constexpr Expected& operator=(ErrorType&& error) noexcept {
    this->variant = std::move(error);
    return *this;
  }

  constexpr bool has() const noexcept { return this->variant.which() == 0; }
  constexpr operator bool() const noexcept { return this->has(); }
  constexpr bool operator!() const noexcept {
    return !static_cast<bool>(*this);
  }

  constexpr ValueType& value() noexcept {
    return const_cast<ValueType&>(
        const_cast<const Expected* const>(this)->value());
  }

  constexpr const ValueType& value() const noexcept {
    CHECK(this->has()) << "Expected does not have value";
    return boost::get<ValueType>(this->variant);
  }

  constexpr ErrorType& error() noexcept {
    return const_cast<ErrorType&>(
        const_cast<const Expected* const>(this)->error());
  }

  constexpr const ErrorType& error() const noexcept {
    CHECK(!this->has()) << "Expected does not have error";
    return boost::get<ErrorType>(this->variant);
  }

private:
  boost::variant<ValueType, ErrorType> variant;
};

template <typename ValueType, typename ErrorType>
constexpr std::ostream&
operator<<(std::ostream& out,
           const Expected<ValueType, ErrorType>& expected) noexcept {
  if (expected) {
    out << "Expected(Value: " << expected.value() << ")";
  } else {
    out << "Expected(Error: " << expected.error() << ")";
  }
  return out;
}

template <typename ValueType, typename ErrorType>
constexpr bool operator==(const Expected<ValueType, ErrorType>& lhs,
                          const Expected<ValueType, ErrorType>& rhs) noexcept {
  if (lhs.has() != rhs.has()) {
    return false;
  }

  if (lhs.has()) {
    return lhs.value() == rhs.value();
  } else {
    return lhs.error() == rhs.error();
  }
}

template <typename ValueType, typename ErrorType>
constexpr bool operator==(const Expected<ValueType, ErrorType>& lhs,
                          const ValueType& rhs) noexcept {
  return lhs.has() && lhs.value() == rhs;
}

template <typename ValueType, typename ErrorType>
constexpr bool operator==(const Expected<ValueType, ErrorType>& lhs,
                          const ErrorType& rhs) noexcept {
  return !lhs.has() && lhs.error() == rhs;
}

namespace internal {

using ExpectedAssert = Expected<int, float>;

static_assert(!std::is_default_constructible<ExpectedAssert>::value,
              "Expected should not be default-constructible");
static_assert(std::is_copy_constructible<ExpectedAssert>::value,
              "Expected should be copy-constructible");
static_assert(std::is_copy_assignable<ExpectedAssert>::value,
              "Expected should be copy-assignable");
static_assert(std::is_move_constructible<ExpectedAssert>::value,
              "Expected should be move-constructible");
static_assert(std::is_move_assignable<ExpectedAssert>::value,
              "Expected should be move-assignable");

} // namespace internal

} // namespace util

#define _CHECK_EXPECTED_IMPL(expected, expr)                                   \
  const auto expected = (expr);                                                \
  if (!expected) CHECK_FAILED(#expr, expected)
#define CHECK_EXPECTED(expr) _CHECK_EXPECTED_IMPL(UNIQUE_VARIABLE_NAME(), expr)

#define _CHECK_EXPECTED_AND_ASSIGN_IMPL(expected, lhs, rhs)                    \
  const auto expected = (rhs);                                                 \
  if (expected)                                                                \
    (lhs) = expected.value();                                                  \
  else                                                                         \
    CHECK_FAILED(#rhs, expected)
#define CHECK_EXPECTED_AND_ASSIGN(lhs, rhs)                                    \
  _CHECK_EXPECTED_AND_ASSIGN_IMPL(UNIQUE_VARIABLE_NAME(), lhs, rhs)

#define _CHECK_EXPECTED_AND_RETURN_IMPL(expected, expr)                        \
  const auto expected = (expr);                                                \
  if (expected)                                                                \
    return expected.value();                                                   \
  else                                                                         \
    CHECK_FAILED(#expr, expected)
#define CHECK_EXPECTED_AND_RETURN(expr)                                        \
  _CHECK_EXPECTED_AND_RETURN_IMPL(UNIQUE_VARIABLE_NAME(), expr)
