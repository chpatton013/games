#include "task/schedule.h"
#include "gtest/gtest.h"

#define ASSERT_NEXT(w, t) \
  do { \
    ASSERT_TRUE((bool)(w)); \
    ASSERT_EQ((w).get()->task, (t)); \
  } while (false)

namespace task {
namespace {

TEST(ScheduleTest, parents) {
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

  Schedule schedule(registry.get());

  ASSERT_FALSE(schedule.empty());
  ASSERT_EQ(schedule.size(), 5);

  {
    auto next = schedule.get();
    ASSERT_NEXT(next, grandchild1);
  }
  ASSERT_FALSE(schedule.empty());
  ASSERT_EQ(schedule.size(), 4);

  {
    auto next = schedule.get();
    ASSERT_NEXT(next, grandchild2);
  }
  ASSERT_FALSE(schedule.empty());
  ASSERT_EQ(schedule.size(), 3);

  {
    auto next = schedule.get();
    ASSERT_NEXT(next, child1);
  }
  ASSERT_FALSE(schedule.empty());
  ASSERT_EQ(schedule.size(), 2);

  {
    auto next = schedule.get();
    ASSERT_NEXT(next, child2);
  }
  ASSERT_FALSE(schedule.empty());
  ASSERT_EQ(schedule.size(), 1);

  {
    auto next = schedule.get();
    ASSERT_NEXT(next, parent);
  }
  ASSERT_TRUE(schedule.empty());
  ASSERT_EQ(schedule.size(), 0);

  ASSERT_EQ(schedule.get(), boost::none);
}

TEST(TaskTest, dependencies) {
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

  Schedule schedule(registry);

  ASSERT_FALSE(schedule.empty());
  ASSERT_EQ(schedule.size(), 4);

  auto a_wrapper = schedule.get();
  ASSERT_NEXT(a_wrapper, a);
  ASSERT_FALSE(schedule.empty());
  ASSERT_EQ(schedule.size(), 3);

  ASSERT_EQ(schedule.get(), boost::none);

  a_wrapper.reset();

  auto b_wrapper = schedule.get();
  ASSERT_NEXT(b_wrapper, b);
  ASSERT_FALSE(schedule.empty());
  ASSERT_EQ(schedule.size(), 2);

  auto c_wrapper = schedule.get();
  ASSERT_NEXT(c_wrapper, c);
  ASSERT_FALSE(schedule.empty());
  ASSERT_EQ(schedule.size(), 1);

  ASSERT_EQ(schedule.get(), boost::none);

  c_wrapper.reset();
  ASSERT_EQ(schedule.get(), boost::none);

  b_wrapper.reset();
  auto d_wrapper = schedule.get();
  ASSERT_NEXT(d_wrapper, d);
  ASSERT_TRUE(schedule.empty());
  ASSERT_EQ(schedule.size(), 0);

  ASSERT_EQ(schedule.get(), boost::none);
}

} // namespace
} // namespace task
