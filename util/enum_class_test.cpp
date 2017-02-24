#include "util/enum_class.h"
#include <gtest/gtest.h>

namespace util {

ENUM_CLASS(TestEnum, A, B, C)

TEST(EnumClassTest, enum_class_values) {
  ASSERT_EQ(static_cast<int>(TestEnum::Enum::A), 0);
  ASSERT_EQ(static_cast<int>(TestEnum::Enum::B), 1);
  ASSERT_EQ(static_cast<int>(TestEnum::Enum::C), 2);

  const auto values = TestEnum::Values();
  ASSERT_EQ(values.size(), 3);
  ASSERT_EQ(values[0], TestEnum::Enum::A);
  ASSERT_EQ(values[1], TestEnum::Enum::B);
  ASSERT_EQ(values[2], TestEnum::Enum::C);
}

TEST(EnumClassTest, enum_class_names) {
  ASSERT_EQ(TestEnum::Name<TestEnum::Enum::A>::key(), TestEnum::Enum::A);
  ASSERT_EQ(TestEnum::Name<TestEnum::Enum::B>::key(), TestEnum::Enum::B);
  ASSERT_EQ(TestEnum::Name<TestEnum::Enum::C>::key(), TestEnum::Enum::C);
  ASSERT_EQ(TestEnum::Name<TestEnum::Enum::A>::value(), "A");
  ASSERT_EQ(TestEnum::Name<TestEnum::Enum::B>::value(), "B");
  ASSERT_EQ(TestEnum::Name<TestEnum::Enum::C>::value(), "C");

  const auto names = TestEnum::Names();
  ASSERT_EQ(names.size(), 3);
  ASSERT_EQ(std::get<0>(names), "A");
  ASSERT_EQ(std::get<1>(names), "B");
  ASSERT_EQ(std::get<2>(names), "C");
}

TEST(EnumClassTest, instance) {
  const auto a = TestEnum::Create<TestEnum::Enum::A>();
  ASSERT_EQ(a.value, TestEnum::Enum::A);
  ASSERT_EQ(a.name, "A");

  const auto b = TestEnum::Create<TestEnum::Enum::B>();
  ASSERT_EQ(b.value, TestEnum::Enum::B);
  ASSERT_EQ(b.name, "B");

  const auto c = TestEnum::Create<TestEnum::Enum::C>();
  ASSERT_EQ(c.value, TestEnum::Enum::C);
  ASSERT_EQ(c.name, "C");

  const auto instances = TestEnum::Instances();
  ASSERT_EQ(instances.size(), 3);
  ASSERT_EQ(std::get<0>(instances), TestEnum::Create<TestEnum::Enum::A>());
  ASSERT_EQ(std::get<1>(instances), TestEnum::Create<TestEnum::Enum::B>());
  ASSERT_EQ(std::get<2>(instances), TestEnum::Create<TestEnum::Enum::C>());

  ASSERT_EQ(TestEnum::Create<std::get<0>(TestEnum::Values())>(),
            TestEnum::Create<TestEnum::Enum::A>());
  ASSERT_EQ(TestEnum::Create<std::get<1>(TestEnum::Values())>(),
            TestEnum::Create<TestEnum::Enum::B>());
  ASSERT_EQ(TestEnum::Create<std::get<2>(TestEnum::Values())>(),
            TestEnum::Create<TestEnum::Enum::C>());
}

} // namespace util
