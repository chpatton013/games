#include "task/task.h"
#include "gtest/gtest.h"

namespace task {
namespace {

TEST(TaskTest, taskParents) {
  auto delegate = [](){};
  auto parent = Task::Create(delegate);
  auto child1 = Task::Create(delegate, kPriorityNormal, parent);
  auto child2 = Task::Create(delegate, kPriorityNormal, parent);
  auto grandchild1 = Task::Create(delegate, kPriorityNormal, child1);
  auto grandchild2 = Task::Create(delegate, kPriorityNormal, child1);

  ASSERT_EQ(parent->parent, boost::none);
  ASSERT_EQ(child1->parent, parent);
  ASSERT_EQ(child2->parent, parent);
  ASSERT_EQ(grandchild1->parent, child1);
  ASSERT_EQ(grandchild2->parent, child1);

  ASSERT_EQ(parent->parentDepth(), 0);
  ASSERT_EQ(child1->parentDepth(), 1);
  ASSERT_EQ(child2->parentDepth(), 1);
  ASSERT_EQ(grandchild1->parentDepth(), 2);
  ASSERT_EQ(grandchild2->parentDepth(), 2);
}

TEST(TaskTest, taskDependencies) {
  auto delegate = [](){};
  auto a = Task::Create(delegate);
  auto b = Task::Create(delegate, kPriorityNormal, boost::none, a);
  auto c = Task::Create(delegate, kPriorityNormal, boost::none, a);
  auto d = Task::Create(delegate, kPriorityNormal, boost::none, b);

  ASSERT_EQ(a->dependency, boost::none);
  ASSERT_EQ(b->dependency.get().lock(), a);
  ASSERT_EQ(c->dependency.get().lock(), a);
  ASSERT_EQ(d->dependency.get().lock(), b);

  ASSERT_EQ(a->dependencyDepth(), 0);
  ASSERT_EQ(b->dependencyDepth(), 1);
  ASSERT_EQ(c->dependencyDepth(), 1);
  ASSERT_EQ(d->dependencyDepth(), 2);
}

TEST(TaskWrapperTest, taskWrapperParents) {
  auto delegate = [](){};
  auto parent = Task::Create(delegate);
  auto child1 = Task::Create(delegate, kPriorityNormal, parent);
  auto child2 = Task::Create(delegate, kPriorityNormal, parent);
  auto grandchild1 = Task::Create(delegate, kPriorityNormal, child1);
  auto grandchild2 = Task::Create(delegate, kPriorityNormal, child1);

  auto parent_wrapper = TaskWrapper::Create(parent);
  auto child1_wrapper = TaskWrapper::Create(child1, parent_wrapper);
  auto child2_wrapper = TaskWrapper::Create(child2, parent_wrapper);
  auto grandchild1_wrapper = TaskWrapper::Create(grandchild1, child1_wrapper);
  auto grandchild2_wrapper = TaskWrapper::Create(grandchild2, child1_wrapper);

  ASSERT_EQ(parent_wrapper->parent, boost::none);
  ASSERT_EQ(child1_wrapper->parent, parent_wrapper);
  ASSERT_EQ(child2_wrapper->parent, parent_wrapper);
  ASSERT_EQ(grandchild1_wrapper->parent, child1_wrapper);
  ASSERT_EQ(grandchild2_wrapper->parent, child1_wrapper);

  ASSERT_TRUE(parent_wrapper->ready());
  ASSERT_TRUE(child1_wrapper->ready());
  ASSERT_TRUE(child2_wrapper->ready());
  ASSERT_TRUE(grandchild1_wrapper->ready());
  ASSERT_TRUE(grandchild2_wrapper->ready());
}

TEST(TaskWrapperTest, taskWrapperDependencies) {
  auto delegate = [](){};
  auto a = Task::Create(delegate);
  auto b = Task::Create(delegate, kPriorityNormal, boost::none, a);
  auto c = Task::Create(delegate, kPriorityNormal, boost::none, a);
  auto d = Task::Create(delegate, kPriorityNormal, boost::none, b);

  auto a_wrapper = TaskWrapper::Create(a);
  auto b_wrapper = TaskWrapper::Create(b, boost::none, a_wrapper);
  auto c_wrapper = TaskWrapper::Create(c, boost::none, a_wrapper);
  auto d_wrapper = TaskWrapper::Create(d, boost::none, b_wrapper);

  ASSERT_EQ(a_wrapper->dependency, boost::none);
  ASSERT_EQ(b_wrapper->dependency.get().lock(), a_wrapper);
  ASSERT_EQ(c_wrapper->dependency.get().lock(), a_wrapper);
  ASSERT_EQ(d_wrapper->dependency.get().lock(), b_wrapper);

  ASSERT_TRUE(a_wrapper->ready());
  ASSERT_FALSE(b_wrapper->ready());
  ASSERT_FALSE(c_wrapper->ready());
  ASSERT_FALSE(d_wrapper->ready());

  a_wrapper.reset();
  ASSERT_TRUE(b_wrapper->ready());
  ASSERT_TRUE(c_wrapper->ready());
  ASSERT_FALSE(d_wrapper->ready());

  c_wrapper.reset();
  ASSERT_TRUE(b_wrapper->ready());
  ASSERT_FALSE(d_wrapper->ready());

  b_wrapper.reset();
  ASSERT_TRUE(d_wrapper->ready());
}

} // namespace
} // namespace task
