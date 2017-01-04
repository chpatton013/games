#include "task/registry.h"
#include "gtest/gtest.h"

#define ASSERT_WRAPPER(w, t) \
  do { \
    ASSERT_EQ((w)->task, (t)); \
    ASSERT_EQ((bool)(w)->parent, (bool)(t)->parent); \
    if ((w)->parent) { \
      ASSERT_EQ((w)->parent.get()->task, (t)->parent.get()); \
    } \
    ASSERT_EQ((bool)(w)->dependency, (bool)(t)->dependency); \
    if ((w)->dependency) { \
      ASSERT_EQ((w)->dependency.get().lock()->task, (t)->dependency.get().lock()); \
    } \
  } while (false)

namespace task {
namespace {

TEST(RegistryTest, parents) {
  auto delegate = [](){};
  auto parent = Task::Create(delegate);
  auto child1 = Task::Create(delegate, kPriorityNormal, parent);
  auto child2 = Task::Create(delegate, kPriorityNormal, parent);
  auto grandchild1 = Task::Create(delegate, kPriorityNormal, child1);
  auto grandchild2 = Task::Create(delegate, kPriorityNormal, child1);

  Registry registry;
  registry.add(parent);
  registry.add(child1);
  registry.add(child2);
  registry.add(grandchild1);
  registry.add(grandchild2);

  const auto wrappers = registry.get();
  ASSERT_EQ(wrappers.size(), 5);
  ASSERT_WRAPPER(wrappers[0], parent);
  ASSERT_WRAPPER(wrappers[1], child1);
  ASSERT_WRAPPER(wrappers[2], child2);
  ASSERT_WRAPPER(wrappers[3], grandchild1);
  ASSERT_WRAPPER(wrappers[4], grandchild2);
}

TEST(RegistryTest, dependencies) {
  auto delegate = [](){};
  auto a = Task::Create(delegate);
  auto b = Task::Create(delegate, kPriorityNormal, boost::none, a);
  auto c = Task::Create(delegate, kPriorityNormal, boost::none, a);
  auto d = Task::Create(delegate, kPriorityNormal, boost::none, b);

  Registry registry;
  registry.add(a);
  registry.add(b);
  registry.add(c);
  registry.add(d);

  const auto wrappers = registry.get();
  ASSERT_EQ(wrappers.size(), 4);
  ASSERT_WRAPPER(wrappers[0], a);
  ASSERT_WRAPPER(wrappers[1], b);
  ASSERT_WRAPPER(wrappers[2], c);
  ASSERT_WRAPPER(wrappers[3], d);
}

} // namespace
} // namespace task
