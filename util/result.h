#pragma once

#include "trait/eq_ops.h"
#include "trait/to_string.h"
#include "util/expected.h"
#include "util/macros.h"
#include "util/static_string.h"
#include <glog/logging.h>
#include <sstream>

namespace util {

// TODO: Support user-supplied error codes.
enum class ErrorCode {
  Ok,
  InvalidValue,
  InvalidIndex,
  AlreadyExists,
  NotFound,
  Unimplemented,
  Unknown,
};

static constexpr StaticString ErrorCodeToName(const ErrorCode& code) noexcept {
  switch (code) {
  case ErrorCode::Ok: return "Ok";
  case ErrorCode::InvalidValue: return "InvalidValue";
  case ErrorCode::InvalidIndex: return "InvalidIndex";
  case ErrorCode::AlreadyExists: return "AlreadyExists";
  case ErrorCode::NotFound: return "NotFound";
  case ErrorCode::Unimplemented: return "Unimplemented";
  case ErrorCode::Unknown: return "Unknown";
  default:
    using UnderlyingCode = typename std::underlying_type<ErrorCode>::type;
    const auto underlying_code = static_cast<UnderlyingCode>(code);
    LOG(FATAL) << "Unrecognized ErrorCode: " << underlying_code;
  }
}

// Forward declaration is required for sucessful overload lookup.
// Otherwise, operator<<(stsd::ostream&, bool) is selected.
struct Result;

inline std::ostream& operator<<(std::ostream& out,
                                const Result& result) noexcept;

inline constexpr bool operator==(const Result& lhs, const Result& rhs) noexcept;

struct Result : public trait::ToString<Result>, public trait::EqOps<Result> {
  static constexpr Result CreateOk() noexcept { return Result(ErrorCode::Ok); }

  constexpr Result(ErrorCode code, StaticString message) noexcept
    : code(std::move(code)), message(std::move(message)) {}
  explicit constexpr Result(ErrorCode code) noexcept
    : Result(std::move(code), "") {}

  constexpr bool ok() const noexcept { return this->code == ErrorCode::Ok; }
  constexpr operator bool() const noexcept { return this->ok(); }
  constexpr bool operator!() const noexcept {
    return !static_cast<bool>(*this);
  }

  const ErrorCode code;
  const StaticString message;
};

static_assert(!std::is_default_constructible<Result>::value,
              "Result should not be default-constructible");
static_assert(std::is_copy_constructible<Result>::value,
              "Result should be copy-constructible");
static_assert(!std::is_copy_assignable<Result>::value,
              "Result should not be copy-assignable");
static_assert(std::is_move_constructible<Result>::value,
              "Result should be move-constructible");
static_assert(!std::is_move_assignable<Result>::value,
              "Result should not be move-assignable");
static_assert(std::is_literal_type<Result>::value,
              "Result should be a literal type");

std::ostream& operator<<(std::ostream& out, const Result& result) noexcept {
  out << ErrorCodeToName(result.code);
  if (!result.message.empty()) {
    out << ": " << result.message;
  }
  return out;
}

constexpr bool operator==(const Result& lhs, const Result& rhs) noexcept {
  return lhs.code == rhs.code && lhs.message == rhs.message;
}

} // namespace util

#define _DO_CHECK_RESULT(result, expr)                                         \
  const auto result = (expr);                                                  \
  if (!result) CHECK_FAILED(#expr, result)
#define CHECK_RESULT(expr) _DO_CHECK_RESULT(UNIQUE_VARIABLE_NAME(), expr)
