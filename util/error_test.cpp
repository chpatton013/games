#include "util/error.h"
#include "gtest/gtest.h"

namespace util {
namespace {

TEST(ErrorTest, ok) {
  {
    const Error e = Error::CreateOk();
    ASSERT_EQ(e.code, ErrorCode::Ok);
    ASSERT_EQ(e.message, "");
    ASSERT_TRUE(e.ok());
    ASSERT_TRUE(static_cast<bool>(e));
    ASSERT_FALSE(!e);
  }

  {
    const Error e(ErrorCode::Ok, "Unique error message");
    ASSERT_EQ(e.code, ErrorCode::Ok);
    ASSERT_EQ(e.message, "Unique error message");
    ASSERT_TRUE(e.ok());
    ASSERT_TRUE(static_cast<bool>(e));
    ASSERT_FALSE(!e);
  }
}

TEST(ErrorTest, notOk) {
  {
    const Error e(ErrorCode::InvalidValue);
    ASSERT_EQ(e.code, ErrorCode::InvalidValue);
    ASSERT_EQ(e.message, "");
    ASSERT_FALSE(e.ok());
    ASSERT_FALSE(static_cast<bool>(e));
    ASSERT_TRUE(!e);
  }

  {
    const Error e(ErrorCode::InvalidValue, "Unique error message");
    ASSERT_EQ(e.code, ErrorCode::InvalidValue);
    ASSERT_EQ(e.message, "Unique error message");
    ASSERT_FALSE(e.ok());
    ASSERT_FALSE(static_cast<bool>(e));
    ASSERT_TRUE(!e);
  }
}

TEST(ErrorTest, defaultErrorMessages) {
  ASSERT_EQ(Error(ErrorCode::Ok).full_message(), "Ok: ");
  ASSERT_EQ(Error(ErrorCode::InvalidValue).full_message(), "InvalidValue: ");
  ASSERT_EQ(Error(ErrorCode::InvalidIndex).full_message(), "InvalidIndex: ");
  ASSERT_EQ(Error(ErrorCode::AlreadyExists).full_message(), "AlreadyExists: ");
  ASSERT_EQ(Error(ErrorCode::NotFound).full_message(), "NotFound: ");
  ASSERT_EQ(Error(ErrorCode::Unimplemented).full_message(), "Unimplemented: ");
  ASSERT_EQ(Error(ErrorCode::Unknown).full_message(), "Unknown: ");
}

TEST(ErrorTest, check) {
  CHECK(Error::CreateOk());
  auto error = Error(ErrorCode::InvalidValue);
  ASSERT_DEATH({ CHECK(error.ok()) << "My message"; },
               ".*Check failed: error\\.ok\\(\\) My message");
}

TEST(ErrorTest, checkOk) {
  CHECK_OK(Error::CreateOk());
  auto error = Error(ErrorCode::InvalidValue);
  ASSERT_DEATH({ CHECK_OK(error) << "My message"; },
               ".*Check failed: \\(error\\)\\.ok\\(\\) InvalidValue:  My message");
}

TEST(ErrorTest, checkAndAssign) {
  {
    int lhs = 5;
    ErrorOr<int> rhs(6);
    CHECK_AND_ASSIGN(lhs, rhs);
    ASSERT_EQ(lhs, 6);
  }

  {
    int lhs = 5;
    ErrorOr<int> rhs = Error(ErrorCode::InvalidValue);
    ASSERT_DEATH({ CHECK_AND_ASSIGN(lhs, rhs) << "My message"; },
                 ".*Check failed: rhs InvalidValue:  My message");
  }
}

template <typename Value>
Value CheckAndReturn(const ErrorOr<Value>& error_or) {
  CHECK_AND_RETURN(error_or);
}

TEST(ErrorTest, checkAndReturn) {
  ASSERT_EQ(CheckAndReturn(ErrorOr<int>(5)), 5);
  auto error_or = ErrorOr<int>(Error(ErrorCode::InvalidValue));
  ASSERT_DEATH(CheckAndReturn(error_or),
               ".*Check failed: error_or InvalidValue:");
}

} // namespace
} // namespace util
