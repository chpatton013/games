#include "trait/rel_ops.h"
#include <gtest/gtest.h>

namespace trait {

struct A final : public RelOps<A> {
  A(int v) : v(v) {}
  const int v;
  bool operator==(const A& that) const { return this->v == that.v; }
  bool operator<(const A& that) const { return this->v < that.v; }
};

struct B final : public RelOps<B> {
  B(int v) : v(v) {}
  const int v;
};

bool operator==(const B& lhs, const B& rhs) { return lhs.v == rhs.v; }
bool operator<(const B& lhs, const B& rhs) { return lhs.v < rhs.v; }

TEST(RelOpsTest, member_functions) {
  const A x(1);
  const A y(1);
  const A z(2);

  ASSERT_TRUE(x == y);
  ASSERT_FALSE(x == z);
  ASSERT_FALSE(x != y);
  ASSERT_TRUE(x != z);
  ASSERT_FALSE(x < y);
  ASSERT_TRUE(x < z);
  ASSERT_FALSE(x > y);
  ASSERT_FALSE(x > z);
  ASSERT_TRUE(x <= y);
  ASSERT_TRUE(x <= z);
  ASSERT_TRUE(x >= y);
  ASSERT_FALSE(x >= z);

  ASSERT_TRUE(y == x);
  ASSERT_FALSE(z == x);
  ASSERT_FALSE(y != x);
  ASSERT_TRUE(z != x);
  ASSERT_FALSE(y > x);
  ASSERT_TRUE(z > x);
  ASSERT_FALSE(y < x);
  ASSERT_FALSE(z < x);
  ASSERT_TRUE(y >= x);
  ASSERT_TRUE(z >= x);
  ASSERT_TRUE(y <= x);
  ASSERT_FALSE(z <= x);
}

TEST(RelOpsTest, free_functions) {
  const B x(1);
  const B y(1);
  const B z(2);

  ASSERT_TRUE(x == y);
  ASSERT_FALSE(x == z);
  ASSERT_FALSE(x != y);
  ASSERT_TRUE(x != z);
  ASSERT_FALSE(x < y);
  ASSERT_TRUE(x < z);
  ASSERT_FALSE(x > y);
  ASSERT_FALSE(x > z);
  ASSERT_TRUE(x <= y);
  ASSERT_TRUE(x <= z);
  ASSERT_TRUE(x >= y);
  ASSERT_FALSE(x >= z);

  ASSERT_TRUE(y == x);
  ASSERT_FALSE(z == x);
  ASSERT_FALSE(y != x);
  ASSERT_TRUE(z != x);
  ASSERT_FALSE(y > x);
  ASSERT_TRUE(z > x);
  ASSERT_FALSE(y < x);
  ASSERT_FALSE(z < x);
  ASSERT_TRUE(y >= x);
  ASSERT_TRUE(z >= x);
  ASSERT_TRUE(y <= x);
  ASSERT_FALSE(z <= x);
}

} // namespace trait
