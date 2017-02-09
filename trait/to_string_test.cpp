#include "trait/to_string.h"
#include <gtest/gtest.h>

namespace trait {

struct A;
std::ostream& operator<<(std::ostream& out, const A& o);

struct A final : public ToString<A> {
  A(int v) : v(v) {}
  const int v;
};

std::ostream& operator<<(std::ostream& out, const A& o) { return out << o.v; }

TEST(ToStringTest, to_string) {
  ASSERT_EQ(A(1).to_string(), "1");
  ASSERT_EQ(A(2).to_string(), "2");
}

} // namespace trait
