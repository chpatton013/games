#include "util/defer.h"
#include <gtest/gtest.h>

namespace util {

TEST(DeferTest, l_value) {
  bool invoked = false;
  {
    const auto defer = Defer([&invoked]() { invoked = true; });
  }
  ASSERT_TRUE(invoked);
}

TEST(DeferTest, r_value) {
  bool invoked = false;
  Defer([&invoked]() { invoked = true; });
  ASSERT_TRUE(invoked);
}

TEST(DeferTest, moved) {
  bool invoked = false;
  Defer d1([&invoked]() { invoked = true; });
  {
    Defer d2 = std::move(d1);
    ASSERT_FALSE(invoked);
  }
  ASSERT_TRUE(invoked);
}

TEST(DeferAllTest, l_value) {
  bool invoked = false;
  {
    const auto defer_all = DeferAll([&invoked]() { invoked = true; });
    ASSERT_EQ(*defer_all.reference_count, 1);
    ASSERT_FALSE(invoked);
  }
  ASSERT_TRUE(invoked);
}

TEST(DeferAllTest, r_value) {
  bool invoked = false;
  DeferAll([&invoked]() { invoked = true; });
  ASSERT_TRUE(invoked);
}

TEST(DeferAllTest, copied) {
  bool invoked = false;
  {
    DeferAll d1([&invoked]() { invoked = true; });
    ASSERT_EQ(*d1.reference_count, 1);
    {
      DeferAll d2 = d1;
      ASSERT_EQ(*d1.reference_count, 2);
      ASSERT_EQ(*d2.reference_count, 2);
      ASSERT_FALSE(invoked);
    }
    ASSERT_EQ(*d1.reference_count, 1);
    ASSERT_FALSE(invoked);
  }
  ASSERT_TRUE(invoked);
}

TEST(DeferAllTest, moved) {
  bool invoked = false;
  DeferAll d1([&invoked]() { invoked = true; });
  ASSERT_EQ(*d1.reference_count, 1);
  {
    DeferAll d2 = std::move(d1);
    ASSERT_EQ(*d2.reference_count, 1);
    ASSERT_FALSE(invoked);
  }
  ASSERT_TRUE(invoked);
}

TEST(DeferAnyTest, l_value) {
  bool invoked = false;
  {
    const auto defer_any = DeferAny([&invoked]() { invoked = true; });
    ASSERT_FALSE(invoked);
  }
  ASSERT_TRUE(invoked);
}

TEST(DeferAnyTest, r_value) {
  bool invoked = false;
  DeferAny([&invoked]() { invoked = true; });
  ASSERT_TRUE(invoked);
}

TEST(DeferAnyTest, copied) {
  bool invoked = false;
  {
    const auto d1 = DeferAny([&invoked]() { invoked = true; });
    {
      const auto d2 = d1;
      ASSERT_FALSE(invoked);
    }
    ASSERT_TRUE(invoked);
    invoked = false;
  }
  ASSERT_FALSE(invoked);
}

TEST(DeferAnyTest, moved) {
  bool invoked = false;
  {
    const auto d1 = DeferAny([&invoked]() { invoked = true; });
    {
      const auto d2 = std::move(d1);
      ASSERT_FALSE(invoked);
    }
    ASSERT_TRUE(invoked);
    invoked = false;
  }
  ASSERT_FALSE(invoked);
}

} // namespace util
