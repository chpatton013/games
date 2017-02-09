#include "util/result.h"
#include <gtest/gtest.h>

namespace util {

TEST(ResultTest, ok) {
  {
    const Result e = Result::CreateOk();
    ASSERT_EQ(e.code, ErrorCode::Ok);
    ASSERT_EQ(e.message, "");
    ASSERT_TRUE(e.ok());
    ASSERT_TRUE(static_cast<bool>(e));
    ASSERT_FALSE(!e);
  }

  {
    const Result e(ErrorCode::Ok, "Unique error message");
    ASSERT_EQ(e.code, ErrorCode::Ok);
    ASSERT_EQ(e.message, "Unique error message");
    ASSERT_TRUE(e.ok());
    ASSERT_TRUE(static_cast<bool>(e));
    ASSERT_FALSE(!e);
  }
}

TEST(ResultTest, notOk) {
  {
    const Result e(ErrorCode::InvalidValue);
    ASSERT_EQ(e.code, ErrorCode::InvalidValue);
    ASSERT_EQ(e.message, "");
    ASSERT_FALSE(e.ok());
    ASSERT_FALSE(static_cast<bool>(e));
    ASSERT_TRUE(!e);
  }

  {
    const Result e(ErrorCode::InvalidValue, "Unique error message");
    ASSERT_EQ(e.code, ErrorCode::InvalidValue);
    ASSERT_EQ(e.message, "Unique error message");
    ASSERT_FALSE(e.ok());
    ASSERT_FALSE(static_cast<bool>(e));
    ASSERT_TRUE(!e);
  }
}

TEST(ResultTest, defaultErrorMessages) {
  ASSERT_EQ(Result(ErrorCode::Ok).to_string(), "Ok");
  ASSERT_EQ(Result(ErrorCode::InvalidValue).to_string(), "InvalidValue");
  ASSERT_EQ(Result(ErrorCode::InvalidIndex).to_string(), "InvalidIndex");
  ASSERT_EQ(Result(ErrorCode::AlreadyExists).to_string(), "AlreadyExists");
  ASSERT_EQ(Result(ErrorCode::NotFound).to_string(), "NotFound");
  ASSERT_EQ(Result(ErrorCode::Unimplemented).to_string(), "Unimplemented");
  ASSERT_EQ(Result(ErrorCode::Unknown).to_string(), "Unknown");
}

TEST(ResultTest, check) {
  CHECK(Result::CreateOk());
  auto error = Result(ErrorCode::InvalidValue);
  ASSERT_DEATH({ CHECK(error.ok()) << "My message"; },
               ".*Check failed: error\\.ok\\(\\) My message");
}

TEST(ResultTest, checkOk) {
  CHECK_RESULT(Result::CreateOk());
  auto error = Result(ErrorCode::InvalidValue);
  ASSERT_DEATH({ CHECK_RESULT(error) << "My message"; },
               ".*Check failed: error: InvalidValue: My message");
}

} // namespace util
