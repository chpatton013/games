#include "util/identifier.h"
#include <gtest/gtest.h>

namespace util {
namespace {

TEST(IdentifierTest, id) {
  {
    Identifier id;

    ASSERT_EQ(id.next(), 1);
    ASSERT_EQ(id.reserve(), 1);
    ASSERT_EQ(id.next(), 2);
  }

  {
    Identifier id(5);

    ASSERT_EQ(id.next(), 5);
    ASSERT_EQ(id.reserve(), 5);
    ASSERT_EQ(id.next(), 6);
  }
}

} // namespace
} // namespace util
