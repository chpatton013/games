#pragma once

#include "task/task.h"
#include "task/registry.h"
#include <algorithm>
#include "boost/optional.hpp"

namespace task {

namespace {

struct HeuristicLess {
  bool operator()(const Task::SharedPtr& lhs, const Task::SharedPtr& rhs) {
    // Higher priority: More important.
    if (lhs->priority != rhs->priority) {
      return lhs->priority > rhs->priority;
    }

    // Fewer dependencies: Likely to be executable faster.
    const auto lhs_dependency_depth = lhs->dependencyDepth();
    const auto rhs_dependency_depth = rhs->dependencyDepth();
    if (lhs_dependency_depth != rhs_dependency_depth) {
      return lhs_dependency_depth < rhs_dependency_depth;
    }

    // More parents: Potentially free up more dependents.
    const auto lhs_parent_depth = lhs->parentDepth();
    const auto rhs_parent_depth = rhs->parentDepth();
    if (lhs_parent_depth != rhs_parent_depth) {
      return lhs_parent_depth > rhs_parent_depth;
    }

    // Fallback
    return lhs->id < rhs->id;
  }

  bool operator()(const TaskWrapper::SharedPtr& lhs,
                  const TaskWrapper::SharedPtr& rhs) {
    return (*this)(lhs->task, rhs->task);
  }
};

} // namespace

class Schedule {
public:
  explicit Schedule(const TaskWrapper::List& tasks)
    : tasks_(tasks) {
    std::sort(this->tasks_.begin(), this->tasks_.end(), HeuristicLess());
  }

  explicit Schedule(Registry& registry) : Schedule(registry.get()) {}

  ~Schedule() = default;
  Schedule() = delete;
  Schedule(const Schedule&) = delete;
  Schedule(Schedule&&) = default;
  Schedule& operator=(const Schedule&) = delete;
  Schedule& operator=(Schedule&&) = default;

  bool empty() const { return this->tasks_.empty(); }
  std::size_t size() const { return this->tasks_.size(); }

  TaskWrapper::SharedPtrOption get() {
    for (auto itr = this->tasks_.begin(); itr != this->tasks_.end(); ++itr) {
      auto task = *itr;
      if (task->ready()) {
        this->tasks_.erase(itr);
        return task;
      }
    }
    return boost::none;
  }

private:
  TaskWrapper::List tasks_;
};

} // namespace task
