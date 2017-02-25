#pragma once

#include <boost/preprocessor.hpp>
#include <glog/logging.h>

#define CHECK_FAILED(expr, value)                                              \
  LOG(FATAL) << "Check failed: " expr ": " << value << ": "

#define UNIQUE_VARIABLE_NAME() BOOST_PP_CAT(_unique_variable_name_, __COUNTER__)
