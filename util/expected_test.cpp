#include "util/expected.h"
#include "gtest/gtest.h"
#include <string>

namespace util {
namespace {

TEST(ExpectedTest, value) {
  const Expected<int, std::string> value(42);
  ASSERT_TRUE(value.has());
  ASSERT_TRUE(static_cast<bool>(value));
  ASSERT_FALSE(!value);
  ASSERT_EQ(value.value(), 42);

  const Expected<int, std::string> copied(value);
  ASSERT_TRUE(copied.has());
  ASSERT_EQ(copied.value(), 42);

  const Expected<int, std::string> moved(std::move(value));
  ASSERT_TRUE(moved.has());
  ASSERT_EQ(moved.value(), 42);

  const Expected<int, std::string> value_copied(42);
  ASSERT_TRUE(value_copied.has());
  ASSERT_EQ(value_copied.value(), 42);

  const Expected<int, std::string> value_moved(std::move(42));
  ASSERT_TRUE(value_moved.has());
  ASSERT_EQ(value_moved.value(), 42);
}

TEST(ExpectedTest, error) {
  const Expected<int, std::string> error("error");
  ASSERT_FALSE(error.has());
  ASSERT_FALSE(static_cast<bool>(error));
  ASSERT_TRUE(!error);
  ASSERT_EQ(error.error(), "error");

  const Expected<int, std::string> copied(error);
  ASSERT_FALSE(copied.has());
  ASSERT_EQ(copied.error(), "error");

  const Expected<int, std::string> moved(std::move(error));
  ASSERT_FALSE(moved.has());
  ASSERT_EQ(moved.error(), "error");

  const Expected<int, std::string> error_copied(std::string("error"));
  ASSERT_FALSE(error_copied.has());
  ASSERT_EQ(error_copied.error(), std::string("error"));

  std::string to_move = "error";
  const Expected<int, std::string> error_moved(std::move(to_move));
  ASSERT_FALSE(error_moved.has());
  ASSERT_EQ(error_moved.error(), std::string("error"));
}

} // namespace
} // namespace util
