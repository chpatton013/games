#include "util/result.h"
#include <gtest/gtest.h>

namespace util {

ENUM_CLASS(TestCode, Ok, Error1, Error2)

TEST(ResultTest, ok) {
  const auto e = Result<TestCode>::Create<TestCode::Enum::Ok>();
  ASSERT_EQ(e.code.value, TestCode::Enum::Ok);
  ASSERT_EQ(e.message, "");
  ASSERT_TRUE(e.ok());
  ASSERT_TRUE(static_cast<bool>(e));
  ASSERT_FALSE(!e);
}

TEST(ResultTest, ok_message) {
  const auto e = Result<TestCode>::Create<TestCode::Enum::Ok>("message");
  ASSERT_EQ(e.code.value, TestCode::Enum::Ok);
  ASSERT_EQ(e.message, "message");
  ASSERT_TRUE(e.ok());
  ASSERT_TRUE(static_cast<bool>(e));
  ASSERT_FALSE(!e);
}

TEST(ResultTest, not_ok) {
  const auto e = Result<TestCode>::Create<TestCode::Enum::Error1>();
  ASSERT_EQ(e.code.value, TestCode::Enum::Error1);
  ASSERT_EQ(e.message, "");
  ASSERT_FALSE(e.ok());
  ASSERT_FALSE(static_cast<bool>(e));
  ASSERT_TRUE(!e);
}

TEST(ResultTest, not_ok_message) {
  const auto e = Result<TestCode>::Create<TestCode::Enum::Error1>("message");
  ASSERT_EQ(e.code.value, TestCode::Enum::Error1);
  ASSERT_EQ(e.message, "message");
  ASSERT_FALSE(e.ok());
  ASSERT_FALSE(static_cast<bool>(e));
  ASSERT_TRUE(!e);
}

TEST(ResultTest, default_error_messages) {
  ASSERT_EQ(Result<TestCode>::Create<TestCode::Enum::Ok>().to_string(), "Ok");
  ASSERT_EQ(Result<TestCode>::Create<TestCode::Enum::Error1>().to_string(),
            "Error1");
  ASSERT_EQ(Result<TestCode>::Create<TestCode::Enum::Error2>().to_string(),
            "Error2");
}

TEST(ResultTest, explicit_error_message) {
  ASSERT_EQ(Result<TestCode>::Create<TestCode::Enum::Ok>("message").to_string(),
            "Ok: message");
}

TEST(ResultTest, macro) {
  ASSERT_EQ(RESULT(TestCode, Ok),
            Result<TestCode>::Create<TestCode::Enum::Ok>());
  ASSERT_EQ(RESULT(TestCode, Error1, "message"),
            Result<TestCode>::Create<TestCode::Enum::Error1>("message"));
}

TEST(ResultTest, check) {
  CHECK(Result<TestCode>::Create<TestCode::Enum::Ok>());
  const auto error = Result<TestCode>::Create<TestCode::Enum::Error1>();
  ASSERT_DEATH({ CHECK(error.ok()) << "My message"; },
               ".*Check failed: error\\.ok\\(\\) My message");
}

TEST(ResultTest, check_result) {
  CHECK_RESULT(Result<TestCode>::Create<TestCode::Enum::Ok>());
  const auto error = Result<TestCode>::Create<TestCode::Enum::Error1>();
  ASSERT_DEATH({ CHECK_RESULT(error) << "My message"; },
               ".*Check failed: error: Error1: My message");
}

} // namespace util
