#include "util/static_string.h"
#include <gtest/gtest.h>

namespace util {

TEST(StaticStringTest, members) {
  const StaticString str("content");

  ASSERT_EQ(str.data, "content");
  ASSERT_EQ(str.size, std::string("content").size());
}

TEST(StaticStringTest, index) {
  const StaticString str("0123");
  ASSERT_EQ(str[0], '0');
  ASSERT_EQ(str[1], '1');
  ASSERT_EQ(str[2], '2');
  ASSERT_EQ(str[3], '3');

  ASSERT_DEATH({ str[4]; }, ".*Index out of range: 4 not in \\[0,3\\]");
}

} // namespace util
