#pragma once

#include "task/task.h"
#include <algorithm>
#include "boost/optional.hpp"

namespace task {

namespace {

struct TaskIdLess {
  bool operator()(const Task::SharedPtr& lhs, const Task::SharedPtr& rhs) {
    return lhs->id < rhs->id;
  }

  bool operator()(const TaskWrapper::SharedPtr& lhs,
                  const TaskWrapper::SharedPtr& rhs) {
    return (*this)(lhs->task, rhs->task);
  }
};

} // namespace

class Registry {
public:
  ~Registry() = default;
  Registry() = default;
  Registry(const Registry&) = default;
  Registry(Registry&&) = default;
  Registry& operator=(const Registry&) = default;
  Registry& operator=(Registry&&) = default;

  void add(const Task::SharedPtr& task) { this->tasks_.push_back(task); }

  TaskWrapper::List get() {
    std::sort(this->tasks_.begin(), this->tasks_.end(), TaskIdLess());

    TaskWrapper::List task_wrappers;
    task_wrappers.reserve(this->tasks_.size());
    const auto weak_wrapper = [](const TaskWrapper::SharedPtrOption& wrapper)
        -> TaskWrapper::WeakPtrOption {
      if (!wrapper) {
        return boost::none;
      }
      return TaskWrapper::WeakPtr(wrapper.get());
    };
    for (const auto& task : this->tasks_) {
      TaskWrapper::SharedPtrOption parent =
          this->findWrapper(task->parent, task_wrappers);
      TaskWrapper::WeakPtrOption dependency =
          weak_wrapper(this->findWrapper(task->dependency, task_wrappers));
      task_wrappers.emplace_back(TaskWrapper::Create(task, parent, dependency));
    }
    return task_wrappers;
  }

private:
  boost::optional<size_t> findIndexOf(const Task::SharedPtr& task) {
    const auto search = std::lower_bound(this->tasks_.begin(),
                                         this->tasks_.end(),
                                         task,
                                         TaskIdLess());
    if (search == this->tasks_.end()) {
      return boost::none;
    }
    return std::distance(this->tasks_.begin(), search);
  }

  TaskWrapper::SharedPtrOption findWrapper(const Task::WeakPtrOption& task,
                                           const TaskWrapper::List& wrappers) {
    if (!task) {
      return boost::none;
    }
    return this->findWrapper(task.get(), wrappers);
  }

  TaskWrapper::SharedPtrOption findWrapper(const Task::SharedPtrOption& task,
                                           const TaskWrapper::List& wrappers) {
    if (!task) {
      return boost::none;
    }
    return this->findWrapper(task.get(), wrappers);
  }

  TaskWrapper::SharedPtrOption findWrapper(const Task::WeakPtr& task,
                                           const TaskWrapper::List& wrappers) {
    const auto task_lock = task.lock();
    if (!task_lock) {
      return boost::none;
    }
    return this->findWrapper(task_lock, wrappers);
  }

  TaskWrapper::SharedPtrOption findWrapper(const Task::SharedPtr& task,
                                           const TaskWrapper::List& wrappers) {
    const auto index = this->findIndexOf(task);
    if (!index) {
      return boost::none;
    }
    if (wrappers.size() <= index.get()) {
      return boost::none;
    }
    return wrappers.at(index.get());
  }

  Task::List tasks_;
};

} // namespace task
