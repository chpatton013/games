#include "util/identifier.h"
#include "gtest/gtest.h"

namespace util {
namespace {

TEST(IdentifierTest, id) {
  {
    Identifier id;

    ASSERT_EQ(id.next(), 0);
    ASSERT_EQ(id.reserve(), 0);
    ASSERT_EQ(id.next(), 1);
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
