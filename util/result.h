#pragma once

#include "trait/eq_ops.h"
#include "trait/to_string.h"
#include "util/enum_class.h"
#include "util/expected.h"
#include "util/macros.h"
#include "util/static_string.h"
#include <ostream>
#include <type_traits>
#include <utility>

namespace util {

// Forward declaration is required for sucessful overload lookup.
// Otherwise, operator<<(stsd::ostream&, bool) is selected.
template <typename Code>
struct Result;

template <typename Code>
inline std::ostream& operator<<(std::ostream& out,
                                const Result<Code>& result) noexcept;

template <typename Code>
inline constexpr bool operator==(const Result<Code>& lhs,
                                 const Result<Code>& rhs) noexcept;

template <typename Code>
struct Result : public trait::ToString<Result<Code>>,
                public trait::EqOps<Result<Code>> {
  template <typename Code::Enum code, typename... Args>
  static constexpr Result Create(Args&&... args) noexcept {
    return Result(Code::template Create<code>(), std::forward<Args>(args)...);
  }

  explicit constexpr Result(Code code, StaticString message = "") noexcept
    : code(std::move(code)), message(std::move(message)) {}

  constexpr bool ok() const noexcept {
    return this->code.value == Code::Enum::Ok;
  }
  constexpr operator bool() const noexcept { return this->ok(); }
  constexpr bool operator!() const noexcept {
    return !static_cast<bool>(*this);
  }

  const Code code;
  const StaticString message;
};

template <typename Code>
std::ostream& operator<<(std::ostream& out,
                         const Result<Code>& result) noexcept {
  out << result.code;
  if (!result.message.empty()) {
    out << ": " << result.message;
  }
  return out;
}

template <typename Code>
constexpr bool operator==(const Result<Code>& lhs,
                          const Result<Code>& rhs) noexcept {
  return lhs.code == rhs.code && lhs.message == rhs.message;
}

namespace internal {

ENUM_CLASS(Code, Ok)

static_assert(!std::is_default_constructible<Result<Code>>::value,
              "Result should not be default-constructible");
static_assert(std::is_copy_constructible<Result<Code>>::value,
              "Result should be copy-constructible");
static_assert(!std::is_copy_assignable<Result<Code>>::value,
              "Result should not be copy-assignable");
static_assert(std::is_move_constructible<Result<Code>>::value,
              "Result should be move-constructible");
static_assert(!std::is_move_assignable<Result<Code>>::value,
              "Result should not be move-assignable");
static_assert(std::is_literal_type<Result<Code>>::value,
              "Result should be a literal type");

} // namespace internal

} // namespace util

// Convenience macro to instantiate Result instances.
#define RESULT(Code, value, ...)                                               \
  Result<Code>::Create<Code::Enum::value>(__VA_ARGS__)

// Check that a result is expression is "ok".
#define _CHECK_RESULT_IMPL(result, expr)                                       \
  const auto result = (expr);                                                  \
  if (!result) CHECK_FAILED(#expr, result)
#define CHECK_RESULT(expr) _CHECK_RESULT_IMPL(UNIQUE_VARIABLE_NAME(), expr)
