#pragma once

#include "util/expected.h"
#include "util/result.h"

namespace util {

template <typename Value>
using ResultOr = Expected<Value, Result>;

} // namespace util
