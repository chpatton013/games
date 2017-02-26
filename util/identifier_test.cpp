#include "util/identifier.h"
#include <gtest/gtest.h>

namespace util {

TEST(IdRangeTest, values) {
  IdRange range(1, 3);
  ASSERT_EQ(range.begin, 1);
  ASSERT_EQ(range.end, 3);
  ASSERT_EQ(range.size(), 2);
  ASSERT_FALSE(range.empty());
}

TEST(IdRangeTest, empty) {
  IdRange range(1, 1);
  ASSERT_EQ(range.size(), 0);
  ASSERT_TRUE(range.empty());
}

TEST(IdRangeTest, reversed) {
  ASSERT_DEATH({ IdRange range(2, 1); },
               ".*Check failed: begin <= end \\(2 vs\\. 1\\) Invalid IdRange");
}

TEST(IdRangeTest, eq_ops) {
  IdRange r1(1, 2);
  IdRange r2(1, 3);
  IdRange r3(2, 3);
  ASSERT_EQ(r1, r1);
  ASSERT_NE(r1, r2);
  ASSERT_NE(r1, r3);
  ASSERT_NE(r2, r1);
  ASSERT_EQ(r2, r2);
  ASSERT_NE(r2, r3);
  ASSERT_NE(r3, r1);
  ASSERT_NE(r3, r2);
  ASSERT_EQ(r3, r3);
}

TEST(IdRangeTest, to_string) {
  IdRange range(1, 3);
  ASSERT_EQ(range.to_string(), "IdRange[1,3)");
}

TEST(IdentifierTest, default_ctor) {
  Identifier id;
  ASSERT_TRUE(id.valid());
  ASSERT_EQ(id.last(), Identifier::InvalidId());
  ASSERT_EQ(id.next(), 1);
}

TEST(IdentifierTest, specify_ctor) {
  Identifier id(5);
  ASSERT_TRUE(id.valid());
  ASSERT_EQ(id.last(), 4);
  ASSERT_EQ(id.next(), 5);
}

TEST(IdentifierTest, reserve) {
  Identifier id;
  ASSERT_EQ(id.reserve(), 1);
  ASSERT_EQ(id.last(), 1);
  ASSERT_EQ(id.next(), 2);
  ASSERT_EQ(id.reserve(), 2);
  ASSERT_EQ(id.last(), 2);
  ASSERT_EQ(id.next(), 3);
}

TEST(IdentifierTest, reserve_range) {
  Identifier id;
  ASSERT_EQ(id.reserve(2), IdRange(1, 3));
  ASSERT_EQ(id.last(), 2);
  ASSERT_EQ(id.next(), 3);
  ASSERT_EQ(id.reserve(2), IdRange(3, 5));
  ASSERT_EQ(id.last(), 4);
  ASSERT_EQ(id.next(), 5);
}

TEST(IdentifierTest, reserve_none) {
  ASSERT_DEATH({ Identifier().reserve(0); },
               ".*Check failed: increment > 0 Cannot reserve 0 elements");
}

TEST(IdentifierTest, move_ctor) {
  Identifier id1(5);
  Identifier id2(std::move(id1));
  ASSERT_FALSE(id1.valid());
  ASSERT_EQ(id2.next(), 5);
}

TEST(IdentifierTest, move_assign) {
  Identifier id1(5);
  Identifier id2 = std::move(id1);
  ASSERT_FALSE(id1.valid());
  ASSERT_EQ(id2.next(), 5);
}

TEST(IdentifierTest, move_same) {
  Identifier id(5);
  const auto remove_name = [](Identifier& id) -> Identifier& { return id; };
  ASSERT_DEATH({ remove_name(id) = std::move(id); },
               ".*Check failed: this != &that Cannot "
               "move-assign from the same instance");
}

TEST(IdentifierTest, move_reserve) {
  Identifier id1(5);
  Identifier id2(std::move(id1));
  ASSERT_FALSE(id1.valid());
  ASSERT_DEATH({ id1.reserve(); },
               ".*Check failed: this->valid\\(\\) Identifier is invalid");
}

TEST(IdentifierTest, move_last) {
  Identifier id1(5);
  Identifier id2(std::move(id1));
  ASSERT_FALSE(id1.valid());
  ASSERT_DEATH({ id1.last(); },
               ".*Check failed: this->valid\\(\\) Identifier is invalid");
}

TEST(IdentifierTest, move_next) {
  Identifier id1(5);
  Identifier id2(std::move(id1));
  ASSERT_FALSE(id1.valid());
  ASSERT_DEATH({ id1.next(); },
               ".*Check failed: this->valid\\(\\) Identifier is invalid");
}

} // namespace util
