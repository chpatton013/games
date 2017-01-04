#pragma once

#include "util/expected.h"
#include <string>
#include <sstream>
#include "glog/logging.h"

namespace util {

enum class ErrorCode {
  Ok,
  InvalidValue,
  InvalidIndex,
  AlreadyExists,
  NotFound,
  Unimplemented,
  Unknown,
};

static std::string ErrorCodeToName(const ErrorCode& code) {
  auto underlying_code =
      static_cast<typename std::underlying_type<ErrorCode>::type>(code);
  switch (code) {
    case ErrorCode::Ok: return "Ok";
    case ErrorCode::InvalidValue: return "InvalidValue";
    case ErrorCode::InvalidIndex: return "InvalidIndex";
    case ErrorCode::AlreadyExists: return "AlreadyExists";
    case ErrorCode::NotFound: return "NotFound";
    case ErrorCode::Unimplemented: return "Unimplemented";
    case ErrorCode::Unknown: return "Unknown";
    default: LOG(FATAL) << "Unrecognized ErrorCode: " << underlying_code;
  }
}

struct Error {
  static Error CreateOk() { return Error(ErrorCode::Ok); }

  Error(const ErrorCode& code, const std::string& message)
    : code(code), message(message) {}
  explicit Error(const ErrorCode& code) : Error(code, "") {}

  ~Error() = default;
  Error() = delete;
  Error(const Error&) = default;
  Error(Error&&) = default;
  Error& operator=(const Error&) = default;
  Error& operator=(Error&&) = default;

  bool ok() const { return this->code == ErrorCode::Ok; }
  operator bool() const { return this->ok(); };
  bool operator!() const { return !static_cast<bool>(*this); }

  std::string full_message() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
  }

  friend std::ostream& operator<<(std::ostream& out, const Error& error);

  const ErrorCode code;
  const std::string message;
};

std::ostream& operator<<(std::ostream& out, const Error& error) {
  return out << ErrorCodeToName(error.code) << ": " << error.message;
}

template <typename Value>
using ErrorOr = Expected<Value, Error>;

} // namespace util

#ifndef CHECK
#define CHECK(expr) if (!(expr)) LOG(FATAL) << "Check failed: " #expr << " "
#endif

#define CHECK_OK(error)                                                        \
  auto e = (error);                                                            \
  if (!e.ok()) LOG(FATAL) << "Check failed: (" #error ").ok() " << e << " "

#define _DO_UNIQUE_VARIABLE_NAME_ELEVATE(x, y) x##y
#define _DO_UNIQUE_VARIABLE_NAME(x, y) _DO_UNIQUE_VARIABLE_NAME_ELEVATE(x, y)
#define _UNIQUE_VARIABLE_NAME()                                                \
  _DO_UNIQUE_VARIABLE_NAME(__unique_variable_name_, __COUNTER__)

#define _DO_CHECK_AND_ASSIGN(error_or, lhs, rhs)                               \
  auto error_or = (rhs);                                                       \
  if (error_or) { (lhs) = error_or.value(); }                                  \
  else LOG(FATAL) << "Check failed: " #rhs " " << error_or.error() << " "

#define CHECK_AND_ASSIGN(lhs, rhs)                                             \
  _DO_CHECK_AND_ASSIGN(_UNIQUE_VARIABLE_NAME(), lhs, rhs)

#define _DO_CHECK_AND_RETURN(error_or, rhs)                                    \
  auto error_or = (rhs);                                                       \
  if (error_or) { return error_or.value(); }                                   \
  else LOG(FATAL) << "Check failed: " #rhs " " << error_or.error() << " "

#define CHECK_AND_RETURN(rhs) _DO_CHECK_AND_RETURN(_UNIQUE_VARIABLE_NAME(), rhs)
