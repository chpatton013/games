#include "util/expected.h"
#include <gtest/gtest.h>
#include <string>

namespace util {

#define ASSERT_VALUE(expected, value)                                          \
  do {                                                                         \
    ASSERT_TRUE((expected).has());                                             \
    ASSERT_TRUE(static_cast<bool>(expected));                                  \
    ASSERT_TRUE(!!(expected));                                                 \
    ASSERT_EQ((expected).value(), (value));                                    \
  } while (false)

#define ASSERT_ERROR(expected, error)                                          \
  do {                                                                         \
    ASSERT_FALSE((expected).has());                                            \
    ASSERT_FALSE(static_cast<bool>(expected));                                 \
    ASSERT_FALSE(!!(expected));                                                \
    ASSERT_EQ((expected).error(), (error));                                    \
  } while (false)

using ExpectedTestType = Expected<int, std::string>;

TEST(ExpectedTest, value) {
  const int value = 42;
  const ExpectedTestType copy_ctor(value);
  const ExpectedTestType copy_assign = value;
  const ExpectedTestType move_ctor(42);
  const ExpectedTestType move_assign = 42;

  ASSERT_VALUE(copy_ctor, value);
  ASSERT_VALUE(copy_assign, value);
  ASSERT_VALUE(move_ctor, value);
  ASSERT_VALUE(move_assign, value);
}

TEST(ExpectedTest, error) {
  const std::string error = "error";
  const ExpectedTestType copy_ctor(error);
  const ExpectedTestType copy_assign = error;
  const ExpectedTestType move_ctor(std::string("error"));
  const ExpectedTestType move_assign = std::string("error");

  ASSERT_ERROR(copy_ctor, error);
  ASSERT_ERROR(copy_assign, error);
  ASSERT_ERROR(move_ctor, error);
  ASSERT_ERROR(move_assign, error);
}

TEST(ExpectedTest, to_string) {
  ASSERT_EQ(ExpectedTestType(42).to_string(), "Expected(Value: 42)");
  ASSERT_EQ(ExpectedTestType("error").to_string(), "Expected(Error: error)");
}

TEST(ExpectedTest, equality) {
  ASSERT_EQ(ExpectedTestType(42), ExpectedTestType(42));
  ASSERT_NE(ExpectedTestType(42), ExpectedTestType("error"));
  ASSERT_EQ(ExpectedTestType("error"), ExpectedTestType("error"));
  ASSERT_NE(ExpectedTestType("error"), ExpectedTestType(42));

  ASSERT_EQ(ExpectedTestType(42), 42);
  ASSERT_NE(ExpectedTestType(42), std::string("error"));
  ASSERT_EQ(ExpectedTestType("error"), std::string("error"));
  ASSERT_NE(ExpectedTestType("error"), 42);
}

TEST(ExpectedTest, check_expected) {
  CHECK_EXPECTED(ExpectedTestType(5));
  const auto error = ExpectedTestType("error");
  ASSERT_DEATH({ CHECK_EXPECTED(error) << "My message"; },
               ".*Check failed: error: Expected\\(Error: error\\): My message");
}

TEST(ExpectedTest, check_expected_and_assign) {
  {
    int lhs = 5;
    ExpectedTestType rhs(6);
    CHECK_EXPECTED_AND_ASSIGN(lhs, rhs);
    ASSERT_EQ(lhs, 6);
  }

  {
    int lhs = 5;
    ExpectedTestType rhs = std::string("error");
    ASSERT_DEATH({ CHECK_EXPECTED_AND_ASSIGN(lhs, rhs) << "My message"; },
                 ".*Check failed: rhs: Expected\\(Error: error\\): My message");
  }
}

int CheckAndReturn(ExpectedTestType expected, const std::string message) {
  CHECK_EXPECTED_AND_RETURN(expected) << message;
}

TEST(ExpectedTest, check_expected_and_return) {
  ASSERT_EQ(CheckAndReturn(ExpectedTestType(5), "My message"), 5);
  ExpectedTestType expected = std::string("error");
  ASSERT_DEATH(
      CheckAndReturn(expected, "My message"),
      ".*Check failed: expected: Expected\\(Error: error\\): My message");
}

} // namespace util
