#pragma once

#define CHECK_FAILED(expr, value)                                              \
  LOG(FATAL) << "Check failed: " expr ": " << value << ": "

#define _DO_UNIQUE_VARIABLE_NAME_ELEVATE(x, y) x##y
#define _DO_UNIQUE_VARIABLE_NAME(x, y) _DO_UNIQUE_VARIABLE_NAME_ELEVATE(x, y)
#define UNIQUE_VARIABLE_NAME()                                                 \
  _DO_UNIQUE_VARIABLE_NAME(__unique_variable_name_, __COUNTER__)
