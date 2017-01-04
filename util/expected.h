#pragma once

#include "boost/variant.hpp"
#include "glog/logging.h"
#include <utility>

namespace util {

template <typename ValueType, typename ErrorType>
class Expected {
public:
  ~Expected() = default;
  Expected() = delete;

  Expected(const Expected&) = default;
  Expected(Expected&&) = default;

  Expected(const ValueType& value) : variant_(value){};
  Expected(ValueType&& value) : variant_(std::move(value)) {}

  Expected(const ErrorType& error) : variant_(error) {}
  Expected(ErrorType&& error) : variant_(std::move(error)) {}

  Expected& operator=(const Expected&) = default;
  Expected& operator=(Expected&&) = default;

  Expected& operator=(const ValueType& value) {
    this->variant_ = value;
    return *this;
  }

  Expected& operator=(ValueType&& value) {
    this->variant_ = std::move(value);
    return *this;
  }

  Expected& operator=(const ErrorType& error) {
    this->variant_ = error;
    return *this;
  }

  Expected& operator=(ErrorType&& error) {
    this->variant_ = std::move(error);
    return *this;
  }

  bool has() const {
    return this->variant_.which() == 0;
  }
  operator bool() const {
    return this->has();
  }
  bool operator!() const {
    return !static_cast<bool>(*this);
  }

  ValueType& value() {
    return const_cast<ValueType&>(
        const_cast<const Expected* const>(this)->value());
  }

  const ValueType& value() const {
    if (!this->has()) {
      LOG(FATAL) << "Expected does not have value";
    }
    return boost::get<ValueType>(this->variant_);
  }

  ErrorType& error() {
    return const_cast<ErrorType&>(
        const_cast<const Expected* const>(this)->error());
  }

  const ErrorType& error() const {
    if (this->has()) {
      LOG(FATAL) << "Expected does not have error";
    }
    return boost::get<ErrorType>(this->variant_);
  }

private:
  boost::variant<ValueType, ErrorType> variant_;
};

} // namespace util
