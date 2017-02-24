#include "util/template_map.h"
#include <gtest/gtest.h>

namespace util {

TEMPLATE_MAP(TestMap1, int, char, (1, 'a'), (2, 'b'), (3, 'c'))
TEMPLATE_MAP(TestMap2, char, int, ('a', 1), ('b', 2), ('c', 3))
TEMPLATE_MAP(TestMap3, int, double, (1, 1), (2, 2), (3, 3))
TEMPLATE_MAP(TestMap4, int, const char*, (1, "one"), (2, "two"), (3, "three"))

TEST(TemplateMapTest, int_to_char) {
  ASSERT_EQ(TestMap1<1>::key(), 1);
  ASSERT_EQ(TestMap1<2>::key(), 2);
  ASSERT_EQ(TestMap1<3>::key(), 3);
  ASSERT_EQ(TestMap1<1>::value(), 'a');
  ASSERT_EQ(TestMap1<2>::value(), 'b');
  ASSERT_EQ(TestMap1<3>::value(), 'c');
}

TEST(TemplateMapTest, char_to_int) {
  ASSERT_EQ(TestMap2<'a'>::key(), 'a');
  ASSERT_EQ(TestMap2<'b'>::key(), 'b');
  ASSERT_EQ(TestMap2<'c'>::key(), 'c');
  ASSERT_EQ(TestMap2<'a'>::value(), 1);
  ASSERT_EQ(TestMap2<'b'>::value(), 2);
  ASSERT_EQ(TestMap2<'c'>::value(), 3);
}

TEST(TemplateMapTest, implicit_type_conversion) {
  ASSERT_EQ(TestMap3<1>::key(), 1.0);
  ASSERT_EQ(TestMap3<2>::key(), 2.0);
  ASSERT_EQ(TestMap3<3>::key(), 3.0);
  ASSERT_EQ(TestMap3<1>::value(), 1.0);
  ASSERT_EQ(TestMap3<2>::value(), 2.0);
  ASSERT_EQ(TestMap3<3>::value(), 3.0);
}

TEST(TemplateMapTest, int_to_string) {
  ASSERT_EQ(TestMap4<1>::key(), 1.0);
  ASSERT_EQ(TestMap4<2>::key(), 2.0);
  ASSERT_EQ(TestMap4<3>::key(), 3.0);
  ASSERT_EQ(TestMap4<1>::value(), "one");
  ASSERT_EQ(TestMap4<2>::value(), "two");
  ASSERT_EQ(TestMap4<3>::value(), "three");
}

} // namespace util
